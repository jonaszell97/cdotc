#ifndef CDOT_IRDEBUGANNOTATEPASS_H
#define CDOT_IRDEBUGANNOTATEPASS_H

namespace llvm {
class Module;
} // namespace llvm

namespace cdot {

void addIRDebugInfo(llvm::Module& M, llvm::StringRef ToFile);

} // namespace cdot

#endif // CDOT_IRDEBUGANNOTATEPASS_H
