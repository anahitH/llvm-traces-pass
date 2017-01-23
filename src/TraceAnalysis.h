#pragma once

#include "llvm/Pass.h"
#include <vector>

namespace llvm {

class BasicBlock;
class Function;
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
    Traces getTraces() const;

private:
    class TraceAnaliserImpl;
    std::unique_ptr<TraceAnaliserImpl> m_impl;
}; // class TraceAnalysisPass

} // end of namespace trace

