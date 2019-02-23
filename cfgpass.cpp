#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"

#include <vector>

namespace {

    struct cfgpass : public llvm::FunctionPass
    {
        static char ID;
        std::vector<llvm::BasicBlock*> succ;
        std::map<llvm::BasicBlock*, std::vector<llvm::BasicBlock*>> prec;

        cfgpass() : llvm::FunctionPass(ID) {}

        void get_succ(llvm::BasicBlock& BB)
        {
            auto TerInst  = BB.getTerminator();
            auto num_succ = TerInst->getNumSuccessors();
            for (auto i=0;i<num_succ;++i)
                succ.push_back(TerInst->getSuccessor(i));
        }

        bool runOnFunction(llvm::Function& F) override
        {
           auto& EntryBB = F.getEntryBlock();
           llvm::errs() << EntryBB.getName();
           auto& ExitBB  = F.back();
           llvm::errs() << ExitBB.getName();
           // iterators are just pointers nothing fancy
           // This will give us the successor of each block
           for (auto PBB = F.begin(),End = F.end(); PBB != End; ++PBB)
           {
             get_succ(*PBB);
             llvm::errs()   << " \n This BB "<< PBB->getName() << " has Successor ";
             for(auto BB : succ)
                {
                    llvm::errs() <<"\n" <<BB->getName();
                    // reference is important
                    auto &Vector = prec[BB];
                    //auto holder = static_cast<llvm::BasicBlock*>((*PBB);
                    Vector.push_back(&(*PBB));
                }
             succ.clear();
           }
            llvm::errs () << "\n This count is : "<<prec.size();
           // Iterate over predecessors 
           for (auto itr : prec)
           {
              llvm::errs() << "\n The BB : " << itr.first->getName() << " has predecessor like ";
              auto &pr = itr.second;
              for(auto bbname:pr)
                {
                    llvm::errs() <<"\n "<< bbname->getName();
                }
           }
           prec.clear(); 
           
           // This represents the linear list of basic blocks maintained by the function
           for (auto StartBB = F.begin(), EndBB = F.end(); StartBB != EndBB; ++StartBB)
           {
               auto prev = (*StartBB).getPrevNode();
               if(!prev)
                continue;
               llvm::errs() << "\n Previous node of this node : "<< StartBB->getName() << "  : is "<< prev->getName();
               auto next = StartBB->getNextNode();
               if(!next)
                continue;
               llvm::errs() << "\n Next node of this node : "<< StartBB->getName() << "  : is "<< next->getName(); 
           }
           
        return false;
        } 
    };
}

char cfgpass::ID  = 'c';
static llvm::RegisterPass<cfgpass> CF("cfg","print prev and next nodes");