#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"

#include <unordered_map>
#include <vector>
#include <algorithm>
#define DEBUG_TYPE "mba"
#include "llvm/Support/Debug.h"

// Value is the top base classes

namespace{
    /*
        Get a def use chain and use def chain of instructions in basicblock.

        LLVM CORE CLASS : 
        Value class maintains a use_list, which models def-use chain of that value.
        Provides : use_list, use_iterators. 

        User class provides getOperands API to facilitate the use-def chain. Operands itself are value that are refering 
        to values.
        Provides : Operand_list, op_iterator.

        Instruction class: Data tracked is Inst Opcode and Parent BasicBlock. Often, dyn_cast into derived classes.
        we can clone the instruction 
    */
    struct UserPass : public llvm::BasicBlockPass
    {
        static char ID;
        UserPass() : BasicBlockPass(ID) {}
        bool runOnBasicBlock(llvm::BasicBlock& BB) override
        {
            llvm::errs() << "\n Visited Block : " <<BB.getName();
            for (auto Inst = BB.begin(),End = BB.end();Inst != End; ++Inst)
            {
                // def use chain - for each definition of value, list of it's uses.
                auto check_return = llvm::dyn_cast<llvm::ReturnInst>(Inst);
                if(check_return)
                    continue;
                auto userlist = Inst->users();
                // standard output -1
                Inst->print(llvm::outs());
                // standard error  -2
                llvm::errs() << "\n Above Instruction Users : "<<"\n";
                for (const auto &i : userlist)
                {
                    auto casted = llvm::dyn_cast<llvm::Instruction>(i);
                    if(!casted)
                        continue;
                    casted->print(llvm::errs());
                    llvm::errs() << "\n";
                }
                llvm::errs() << "\n";
                
                //use def chain
                auto useslist = Inst->operands();
                llvm::errs() << *Inst << " : This Instruction Uses : "<<"\n";
                for(auto& j : useslist)
                {
                    llvm::Value* v = j.get();
                    auto casted1 = llvm::dyn_cast<llvm::Instruction>(v);
                    if(!casted1)
                        continue;
                    auto return_ = llvm::dyn_cast<llvm::ReturnInst>(casted1);
                    if (return_)
                        continue;
                    casted1->print(llvm::errs());
                    llvm::errs() << "\n";
                }

            }
        return false;
        } 
    };
}
char UserPass::ID = 'D';
static llvm::RegisterPass<UserPass> chain("ch","Just print Def-Use and Use-def Chains");