#pragma once

#include "llvm/Pass.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace llvm {

class BasicBlock;
class Function;
class LoopInfo;
class LoopInfoWrapperPass;
} // namespace llvm

namespace trace {

class TraceAnalysisPass : public llvm::FunctionPass
{
public:
    typedef std::vector<llvm::BasicBlock*> Trace;
    typedef std::vector<Trace> Traces;

public:
    static char ID;

public:
    TraceAnalysisPass()
            : llvm::FunctionPass(ID)
    {
    }

    void getAnalysisUsage(llvm::AnalysisUsage& AU) const
    {
        AU.addRequired<llvm::LoopInfoWrapperPass>();
        AU.setPreservesAll();
    }

public:
   bool runOnFunction(llvm::Function& F); 

public:
    Traces getTraces(llvm::Function* F) const;

private:
    void dfs(llvm::BasicBlock* BB, Trace trace = Trace());
    void finishCurrentLoop(llvm::BasicBlock* BB, Trace currentTrace);

    void dump() const;
    void scc_dump() const;

private:
    std::unordered_map<llvm::Function*, Traces> m_traces;
    llvm::Function* m_F;
    llvm::LoopInfo* LI;
    std::unordered_set<llvm::BasicBlock*> markedLoopHeaders;
}; // class TraceAnalysisPass

} // end of namespace trace

