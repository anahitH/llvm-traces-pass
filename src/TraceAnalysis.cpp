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

using llvm::Function;
using llvm::BasicBlock;

typedef llvm::scc_iterator<llvm::Inverse<BasicBlock*>> BB_scc_iterator;;

namespace trace {

char TraceAnalysisPass::ID = 0;

bool TraceAnalysisPass::runOnFunction(Function& F)
{
    llvm::errs() << F.getName() << "\n";
    if (F.getName() != "main") {
        return false;
    }
    m_F = &F;
    LI = &getAnalysis<llvm::LoopInfoWrapperPass>().getLoopInfo();

    llvm::errs() << "**** " << F.getName() << "\n ";
    dfs(&*F.begin());
    //scc_dump();
    dump();
    llvm::errs() << "*****\n"; 
    return false;
}

TraceAnalysisPass::Traces TraceAnalysisPass::getTraces(llvm::Function* F) const
{
    const auto& pos = m_traces.find(F);
    if (pos == m_traces.end()) {
        return Traces();
    }
    return pos->second;
}

void TraceAnalysisPass::dump() const
{
    const auto& pos = m_traces.find(m_F);
    if (pos == m_traces.end()) {
        return;
    }
    for (const auto& trace : pos->second) {
        for (const auto& bb : trace) {
            llvm::errs() << bb->getName() << " ";
        }
        llvm::errs() << "\n";
    }
}

void TraceAnalysisPass::dfs_loop(BasicBlock* BB, Trace currentTrace)
{
//    llvm::errs() << "Loop with header " << BB->getName() << "\n";
//    currentTrace.push_back(BB);
//    assert(LI->isLoopHeader(BB));
//    auto loop = LI->getLoopFor(BB);
//    auto header = loop->getHeader();
//    assert(header == BB);
//    for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
//            currentTrace.push_back(*it);
//            return;
//        }
//        //if (loop->isLoopExiting(*it)) {
//        //    currentTrace.push_back(*it);
//        //}
//        dfs(*it, currentTrace);
//    }
}

void TraceAnalysisPass::finishCurrentLoop(BasicBlock* BB, Trace currentTrace)
{
    assert(LI->isLoopHeader(BB));
    auto loop = LI->getLoopFor(BB);
    for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
        if (!loop->contains(*it)) {
        //if (loop->isLoopExiting(*it) && !loop->isLoopLatch(*it)) {
            dfs(*it, currentTrace);
        }
    } 
}

void TraceAnalysisPass::dfs(BasicBlock* BB, Trace currentTrace)
{
    llvm::errs() << BB->getName() << "\n";
    currentTrace.push_back(BB);
    if (llvm::succ_empty(BB)) {
        m_traces[m_F].push_back(currentTrace);
        currentTrace.clear();
        return;
    }
    for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
        if (LI->isLoopHeader(*it)) {
            auto loop = LI->getLoopFor(*it);
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

void TraceAnalysisPass::scc_dump() const
{
    BasicBlock* BB = &m_F->getBasicBlockList().back();
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

/*
// passing Trace by value is a BAD idea
void TraceAnalysisPass::dfs(BasicBlock* BB, Trace currentTrace)
{
    currentTrace.push_back(BB);
    // Supposig the only bb with no successor is end
    //if (llvm::succ_empty(BB)) {
    //    llvm::errs() << "askdjskjf\n";
    //    m_traces[m_F].push_back(currentTrace);
    //    currentTrace.clear();
    //    return;
    //}
    BB = &m_F->getBasicBlockList().back();
    auto it = BB_scc_iterator::begin(BB);
    while (it != BB_scc_iterator::end(BB)) {
        llvm::errs() << "-------------------\n";
        for (auto bb : *it) {
            currentTrace.push_back(bb);
            llvm::errs() << bb->getName();
        }
       // if (it->size() == 1) {
       //     dfs(it->front(), currentTrace);
       // }
        ++it;
        llvm::errs() << "----------------------\n";
    }
        if (it.isAtEnd()) {
            llvm::errs() << "is at end\n";
        }

    m_traces[m_F].push_back(currentTrace);
    currentTrace.clear();
    return;

    //for (auto it = llvm::succ_begin(BB); it != llvm::succ_end(BB); ++it) {
    //    //currentTrace.push_back(*it);
    //    dfs(*it, currentTrace);
    //}
}
*/

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

