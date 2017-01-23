#include "TraceAnalysis.h"

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/SCCIterator.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <cassert>
#include <unordered_set>

using llvm::Function;
using llvm::BasicBlock;

typedef llvm::scc_iterator<llvm::Inverse<BasicBlock*>> BB_scc_iterator;;

namespace trace {


class TraceAnalysisPass::TraceAnaliserImpl
{
private:
    typedef TraceAnalysisPass::Trace Trace;
    typedef TraceAnalysisPass::Traces Traces;

public:
    TraceAnaliserImpl(const llvm::LoopInfo& loopInfo)
        : m_LI(loopInfo)
    {
    }

public:
    void analyze(Function& F);

    const Traces& getTraces() const
    {
        return m_traces;
    }

private:
    void dfs(llvm::BasicBlock* BB, Trace trace = Trace());
    void finishCurrentLoop(llvm::BasicBlock* BB, Trace currentTrace);

    void dump(llvm::Function& F) const;
    void scc_dump(llvm::Function& F) const;

private:
    const llvm::LoopInfo& m_LI;
    Traces m_traces; 
    std::unordered_set<llvm::BasicBlock*> markedLoopHeaders;
};

void TraceAnalysisPass::TraceAnaliserImpl::analyze(Function& F)
{
    dfs(&*F.begin());
    dump(F);
}

void TraceAnalysisPass::TraceAnaliserImpl::dfs(BasicBlock* BB, Trace currentTrace)
{
    //llvm::errs() << BB->getName() << "\n";
    currentTrace.push_back(BB);
    if (llvm::succ_empty(BB)) {
        m_traces.push_back(currentTrace);
        currentTrace.clear();
        return;
    }
    for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
        if (m_LI.isLoopHeader(*it)) {
            auto loop = m_LI.getLoopFor(*it);
            bool newHeader = markedLoopHeaders.insert(*it).second;
            if (!newHeader && loop->contains(BB)) {
                currentTrace.push_back(*it);
                finishCurrentLoop(*it, currentTrace);
                //markedLoopHeaders.erase(*it);
                continue;
            }
        }
        dfs(*it, currentTrace);
    }
}

void TraceAnalysisPass::TraceAnaliserImpl::finishCurrentLoop(BasicBlock* BB, Trace currentTrace)
{
    assert(m_LI.isLoopHeader(BB));
    auto loop = m_LI.getLoopFor(BB);
    for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
        if (!loop->contains(*it)) {
            dfs(*it, currentTrace);
        }
    } 
}

void TraceAnalysisPass::TraceAnaliserImpl::dump(Function& F) const
{

    llvm::errs() << "Traces of function " << F.getName() << "\n";
    for (const auto& trace : m_traces) {
        for (const auto& bb : trace) {
            llvm::errs() << bb->getName() << " ";
        }
        llvm::errs() << "\n";
    }
}

void TraceAnalysisPass::TraceAnaliserImpl::scc_dump(Function& F) const
{
    BasicBlock* BB = &F.getBasicBlockList().back();
    auto it = BB_scc_iterator::begin(BB);
    while (it != BB_scc_iterator::end(BB)) {
        llvm::errs() << "SCC --- ";
        for (auto bb : *it) {
            llvm::errs() << bb->getName() << " ";
        }
        ++it;
        llvm::errs() << "\n";
    }
}


char TraceAnalysisPass::ID = 0;

bool TraceAnalysisPass::runOnFunction(Function& F)
{
    m_impl.reset(new TraceAnaliserImpl(getAnalysis<llvm::LoopInfoWrapperPass>().getLoopInfo()));
    m_impl->analyze(F);
    return false;
}

TraceAnalysisPass::Traces TraceAnalysisPass::getTraces() const
{
    return m_impl->getTraces();
}

// Automatically enable the pass.
static void registerTracePass(const llvm::PassManagerBuilder &,
                              llvm::legacy::PassManagerBase &PM) {
  PM.add(new llvm::LoopInfoWrapperPass());
  PM.add(new TraceAnalysisPass());
}

static llvm::RegisterStandardPasses
  RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                 registerTracePass);

} // namespace trace

