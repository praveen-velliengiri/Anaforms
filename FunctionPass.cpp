
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/ValueSymbolTable.h"

// New LLVM Pass Manager

// CRTP:
/*
    template<typename Derived>      ->  CRTP Class provides the General API && it's implementation 
    class Base{};                       by using common api's of derived template parameter.
                                        (must know the api of derived type)

    class Derived : public Base<Derived>
    {};
*/

// Mix-in 
/*
    template<typename TypeToExtendAPI>
    class Mix-in : public TypeToExtendAPI  -> Extends the API of TypeToExtendAPI type by using TypeToExtendAPI API's 
    {};                                       (must know the api's of TypeToExtendAPI class)
*/

// PassInfoMixin - provides name() API which returns StringRef whose value is Name of the pass 

// Pass Plugin 
struct HelloNewPass : public llvm::PassInfoMixin<HelloNewPass>
{
    private:
        std::map<llvm::StringRef,std::size_t> BBinstcount; 
    public:
    void print() 
    {
        for (auto i=BBinstcount.begin(),j=BBinstcount.end(); i!=j; ++i)
            {
                llvm::outs() << "\n";
                llvm::outs() <<  (*i).first << " : "<< (*i).second;
            }
    }
    llvm::PreservedAnalyses run(llvm::Function& F,llvm::FunctionAnalysisManager& FAM)
    {
        if(F.hasName())
            llvm::errs() << "Function name : " << F.getName() << "Function Inst Count : " <<F.getInstructionCount() <<"\n";
        F.viewCFGOnly();
        // Playing with Function arguments 
        if(!F.arg_empty())
            {
                std::size_t numargs = F.arg_size();
                for(auto argbegin = F.arg_begin(), argend = F.arg_end(); argbegin != argend; ++argbegin)
                    {
                      argbegin->print(llvm::outs());
                    }
            }
        llvm::outs() << "\n";
        auto Fsymtab = F.getValueSymbolTable();
        // Valuesymboltable contains valuemap 
        for(auto f = Fsymtab->begin(),e = Fsymtab->end(); f!=e; ++f)
        {
            llvm::outs() << f->first();
            llvm::outs() << " : name from symbol table  : \n";
            llvm::outs() << " corresponding value : \n";
            f->second->print(llvm::outs());
        }
        auto& BBlist = F.getBasicBlockList();
    
        for (auto first = BBlist.begin(),last = BBlist.end(); first != last; ++first)
        {
            BBinstcount.insert({first->getName(),first->size()});
            if (first->hasName())
                llvm::errs() << first->getName();

        }
        llvm::outs() << "\n";
        print();
        return llvm::PreservedAnalyses::all();
    }
};

// Pass registration - why extern "C" 
extern "C"::llvm::PassPluginLibraryInfo   LLVM_ATTRIBUTE_WEAK
// This function the entry point interface used by the driver 
llvmGetPassPluginInfo(){
    return{ 
        // This constructs and returns a instance of PassPluginLibraryInfo 
    LLVM_PLUGIN_API_VERSION, "HelloNewPass","v0.1",

    //What is a callback-style of programming ?
    [](llvm::PassBuilder& PB) { // callback to register pass
        PB.registerPipelineParsingCallback(
            [](llvm::StringRef PassName,llvm::FunctionPassManager& FPM,llvm::ArrayRef<llvm::PassBuilder::PipelineElement>){
                if (PassName == "hello-new-pm-pass"){
                    FPM.addPass(HelloNewPass());
                    return true;
                }
                return false;
            }
        );
    }
};
}
