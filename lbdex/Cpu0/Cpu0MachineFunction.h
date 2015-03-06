//===-- Cpu0MachineFunctionInfo.h - Private data used for Cpu0 ----*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpu0 specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//

#ifndef CPU0_MACHINE_FUNCTION_INFO_H
#define CPU0_MACHINE_FUNCTION_INFO_H

#include "Cpu0Config.h"
#if CH >= CH3_1

#include "llvm/ADT/StringMap.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include <map>
#include <string>
#include <utility>

namespace llvm {

/// \brief A class derived from PseudoSourceValue that represents a GOT entry
/// resolved by lazy-binding.
class Cpu0CallEntry : public PseudoSourceValue {
public:
  explicit Cpu0CallEntry(const StringRef &N);
  explicit Cpu0CallEntry(const GlobalValue *V);
  bool isConstant(const MachineFrameInfo *) const override;
  bool isAliased(const MachineFrameInfo *) const override;
  bool mayAlias(const MachineFrameInfo *) const override;

private:
  void printCustom(raw_ostream &O) const override;
#ifndef NDEBUG
  std::string Name;
  const GlobalValue *Val;
#endif
};

/// Cpu0FunctionInfo - This class is derived from MachineFunction private
/// Cpu0 target-specific information for each MachineFunction.
class Cpu0FunctionInfo : public MachineFunctionInfo {
public:
  Cpu0FunctionInfo(MachineFunction& MF)
  : MF(MF), 
#if CH >= CH3_4 //1
    SRetReturnReg(0),
#endif
#if CH >= CH6_1 //1
    GlobalBaseReg(0),
#endif
#if CH >= CH3_1
    VarArgsFrameIndex(0), 
#endif
#if CH >= CH9_1
    InArgFIRange(std::make_pair(-1, 0)),
    OutArgFIRange(std::make_pair(-1, 0)), GPFI(0), DynAllocFI(0),
#endif
#if CH >= CH3_2
    EmitNOAT(false), 
#endif
    MaxCallFrameSize(0)
    {}

  ~Cpu0FunctionInfo();

#if CH >= CH9_1
  bool isInArgFI(int FI) const {
    return FI <= InArgFIRange.first && FI >= InArgFIRange.second;
  }
  void setLastInArgFI(int FI) { InArgFIRange.second = FI; }
  bool isOutArgFI(int FI) const {
    return FI <= OutArgFIRange.first && FI >= OutArgFIRange.second;
  }
#endif

#if CH >= CH9_1
  int getGPFI() const { return GPFI; }
  void setGPFI(int FI) { GPFI = FI; }
  bool isGPFI(int FI) const { return GPFI && GPFI == FI; }
#if CH >= CH9_3
#ifdef ENABLE_GPRESTORE
  bool needGPSaveRestore() const { return getGPFI(); }
#endif
#endif

  bool isDynAllocFI(int FI) const { return DynAllocFI && DynAllocFI == FI; }
#endif

#if CH >= CH3_4 //2
  unsigned getSRetReturnReg() const { return SRetReturnReg; }
  void setSRetReturnReg(unsigned Reg) { SRetReturnReg = Reg; }
#endif

#if CH >= CH6_1 //2
  bool globalBaseRegFixed() const;
  bool globalBaseRegSet() const;
  unsigned getGlobalBaseReg();
#endif

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

#if CH >= CH3_4 //3
  bool hasByvalArg() const { return HasByvalArg; }
  void setFormalArgInfo(unsigned Size, bool HasByval) {
    IncomingArgSize = Size;
    HasByvalArg = HasByval;
  }
#endif

#if CH >= CH3_5
  unsigned getIncomingArgSize() const { return IncomingArgSize; }

  unsigned getMaxCallFrameSize() const { return MaxCallFrameSize; }
  void setMaxCallFrameSize(unsigned S) { MaxCallFrameSize = S; }
#endif
#if CH >= CH3_2
  bool getEmitNOAT() const { return EmitNOAT; }
  void setEmitNOAT() { EmitNOAT = true; }
#endif

#if CH >= CH9_2
  /// \brief Create a MachinePointerInfo that has a Cpu0CallEntr object
  /// representing a GOT entry for an external function.
  MachinePointerInfo callPtrInfo(const StringRef &Name);

  /// \brief Create a MachinePointerInfo that has a Cpu0CallEntr object
  /// representing a GOT entry for a global function.
  MachinePointerInfo callPtrInfo(const GlobalValue *Val);
#endif

private:
  virtual void anchor();

  MachineFunction& MF;
#if CH >= CH3_4 //4
  /// SRetReturnReg - Some subtargets require that sret lowering includes
  /// returning the value of the returned struct in a register. This field
  /// holds the virtual register into which the sret argument is passed.
  unsigned SRetReturnReg;
#endif

#if CH >= CH6_1 //3
  /// GlobalBaseReg - keeps track of the virtual register initialized for
  /// use as the global base register. This is used for PIC in some PIC
  /// relocation models.
  unsigned GlobalBaseReg;
#endif

#if CH >= CH3_1
    /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;
#endif

#if CH >= CH3_4 //5
  /// True if function has a byval argument.
  bool HasByvalArg;

  /// Size of incoming argument area.
  unsigned IncomingArgSize;
#endif

#if CH >= CH9_1
  // Range of frame object indices.
  // InArgFIRange: Range of indices of all frame objects created during call to
  //               LowerFormalArguments.
  // OutArgFIRange: Range of indices of all frame objects created during call to
  //                LowerCall except for the frame object for restoring $gp.
  std::pair<int, int> InArgFIRange, OutArgFIRange;
#endif

#if CH >= CH6_1 //4
  int GPFI; // Index of the frame object for restoring $gp
#endif
#if CH >= CH9_1
  mutable int DynAllocFI; // Frame index of dynamically allocated stack area.
#endif
  unsigned MaxCallFrameSize;
#if CH >= CH3_2
  bool EmitNOAT;
#endif

  /// Cpu0CallEntry maps.
  StringMap<const Cpu0CallEntry *> ExternalCallEntries;
  ValueMap<const GlobalValue *, const Cpu0CallEntry *> GlobalCallEntries;
};

} // end of namespace llvm

#endif // #if CH >= CH3_1

#endif // CPU0_MACHINE_FUNCTION_INFO_H

