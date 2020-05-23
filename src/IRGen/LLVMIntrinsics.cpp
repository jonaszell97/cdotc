
#include "cdotc/IRGen/IRGen.h"

#include <llvm/ADT/StringSwitch.h>

using namespace cdot;
using namespace cdot::il;

llvm::Intrinsic::ID IRGen::getIntrinsicID(StringRef Name)
{
   auto It = IntrinsicIDs.find(Name);
   if (It != IntrinsicIDs.end()) {
      return It->getValue();
   }

   auto ID = llvm::StringSwitch<llvm::Intrinsic::ID>(Name)
       .Case("llvm.addressofreturnaddress", llvm::Intrinsic::addressofreturnaddress)
       .Case("llvm.adjust.trampoline", llvm::Intrinsic::adjust_trampoline)
       .Case("llvm.annotation", llvm::Intrinsic::annotation)
       .Case("llvm.assume", llvm::Intrinsic::assume)
       .Case("llvm.bitreverse", llvm::Intrinsic::bitreverse)
       .Case("llvm.bswap", llvm::Intrinsic::bswap)
       .Case("llvm.canonicalize", llvm::Intrinsic::canonicalize)
       .Case("llvm.ceil", llvm::Intrinsic::ceil)
       .Case("llvm.clear_cache", llvm::Intrinsic::clear_cache)
       .Case("llvm.codeview.annotation", llvm::Intrinsic::codeview_annotation)
       .Case("llvm.convert.from.fp16", llvm::Intrinsic::convert_from_fp16)
       .Case("llvm.convert.to.fp16", llvm::Intrinsic::convert_to_fp16)
       .Case("llvm.copysign", llvm::Intrinsic::copysign)
       .Case("llvm.coro.alloc", llvm::Intrinsic::coro_alloc)
       .Case("llvm.coro.alloca.alloc", llvm::Intrinsic::coro_alloca_alloc)
       .Case("llvm.coro.alloca.free", llvm::Intrinsic::coro_alloca_free)
       .Case("llvm.coro.alloca.get", llvm::Intrinsic::coro_alloca_get)
       .Case("llvm.coro.begin", llvm::Intrinsic::coro_begin)
       .Case("llvm.coro.destroy", llvm::Intrinsic::coro_destroy)
       .Case("llvm.coro.done", llvm::Intrinsic::coro_done)
       .Case("llvm.coro.end", llvm::Intrinsic::coro_end)
       .Case("llvm.coro.frame", llvm::Intrinsic::coro_frame)
       .Case("llvm.coro.free", llvm::Intrinsic::coro_free)
       .Case("llvm.coro.id", llvm::Intrinsic::coro_id)
       .Case("llvm.coro.id.retcon", llvm::Intrinsic::coro_id_retcon)
       .Case("llvm.coro.id.retcon.once", llvm::Intrinsic::coro_id_retcon_once)
       .Case("llvm.coro.noop", llvm::Intrinsic::coro_noop)
       .Case("llvm.coro.param", llvm::Intrinsic::coro_param)
       .Case("llvm.coro.prepare.retcon", llvm::Intrinsic::coro_prepare_retcon)
       .Case("llvm.coro.promise", llvm::Intrinsic::coro_promise)
       .Case("llvm.coro.resume", llvm::Intrinsic::coro_resume)
       .Case("llvm.coro.save", llvm::Intrinsic::coro_save)
       .Case("llvm.coro.size", llvm::Intrinsic::coro_size)
       .Case("llvm.coro.subfn.addr", llvm::Intrinsic::coro_subfn_addr)
       .Case("llvm.coro.suspend", llvm::Intrinsic::coro_suspend)
       .Case("llvm.coro.suspend.retcon", llvm::Intrinsic::coro_suspend_retcon)
       .Case("llvm.cos", llvm::Intrinsic::cos)
       .Case("llvm.ctlz", llvm::Intrinsic::ctlz)
       .Case("llvm.ctpop", llvm::Intrinsic::ctpop)
       .Case("llvm.cttz", llvm::Intrinsic::cttz)
       .Case("llvm.dbg.addr", llvm::Intrinsic::dbg_addr)
       .Case("llvm.dbg.declare", llvm::Intrinsic::dbg_declare)
       .Case("llvm.dbg.label", llvm::Intrinsic::dbg_label)
       .Case("llvm.dbg.value", llvm::Intrinsic::dbg_value)
       .Case("llvm.debugtrap", llvm::Intrinsic::debugtrap)
       .Case("llvm.donothing", llvm::Intrinsic::donothing)
       .Case("llvm.eh.dwarf.cfa", llvm::Intrinsic::eh_dwarf_cfa)
       .Case("llvm.eh.exceptioncode", llvm::Intrinsic::eh_exceptioncode)
       .Case("llvm.eh.exceptionpointer", llvm::Intrinsic::eh_exceptionpointer)
       .Case("llvm.eh.recoverfp", llvm::Intrinsic::eh_recoverfp)
       .Case("llvm.eh.return.i32", llvm::Intrinsic::eh_return_i32)
       .Case("llvm.eh.return.i64", llvm::Intrinsic::eh_return_i64)
       .Case("llvm.eh.sjlj.callsite", llvm::Intrinsic::eh_sjlj_callsite)
       .Case("llvm.eh.sjlj.functioncontext", llvm::Intrinsic::eh_sjlj_functioncontext)
       .Case("llvm.eh.sjlj.longjmp", llvm::Intrinsic::eh_sjlj_longjmp)
       .Case("llvm.eh.sjlj.lsda", llvm::Intrinsic::eh_sjlj_lsda)
       .Case("llvm.eh.sjlj.setjmp", llvm::Intrinsic::eh_sjlj_setjmp)
       .Case("llvm.eh.sjlj.setup.dispatch", llvm::Intrinsic::eh_sjlj_setup_dispatch)
       .Case("llvm.eh.typeid.for", llvm::Intrinsic::eh_typeid_for)
       .Case("llvm.eh.unwind.init", llvm::Intrinsic::eh_unwind_init)
       .Case("llvm.exp", llvm::Intrinsic::exp)
       .Case("llvm.exp2", llvm::Intrinsic::exp2)
       .Case("llvm.expect", llvm::Intrinsic::expect)
       .Case("llvm.experimental.constrained.ceil", llvm::Intrinsic::experimental_constrained_ceil)
       .Case("llvm.experimental.constrained.cos", llvm::Intrinsic::experimental_constrained_cos)
       .Case("llvm.experimental.constrained.exp", llvm::Intrinsic::experimental_constrained_exp)
       .Case("llvm.experimental.constrained.exp2", llvm::Intrinsic::experimental_constrained_exp2)
       .Case("llvm.experimental.constrained.fadd", llvm::Intrinsic::experimental_constrained_fadd)
       .Case("llvm.experimental.constrained.fcmp", llvm::Intrinsic::experimental_constrained_fcmp)
       .Case("llvm.experimental.constrained.fcmps", llvm::Intrinsic::experimental_constrained_fcmps)
       .Case("llvm.experimental.constrained.fdiv", llvm::Intrinsic::experimental_constrained_fdiv)
       .Case("llvm.experimental.constrained.floor", llvm::Intrinsic::experimental_constrained_floor)
       .Case("llvm.experimental.constrained.fma", llvm::Intrinsic::experimental_constrained_fma)
       .Case("llvm.experimental.constrained.fmul", llvm::Intrinsic::experimental_constrained_fmul)
       .Case("llvm.experimental.constrained.fpext", llvm::Intrinsic::experimental_constrained_fpext)
       .Case("llvm.experimental.constrained.fptosi", llvm::Intrinsic::experimental_constrained_fptosi)
       .Case("llvm.experimental.constrained.fptoui", llvm::Intrinsic::experimental_constrained_fptoui)
       .Case("llvm.experimental.constrained.fptrunc", llvm::Intrinsic::experimental_constrained_fptrunc)
       .Case("llvm.experimental.constrained.frem", llvm::Intrinsic::experimental_constrained_frem)
       .Case("llvm.experimental.constrained.fsub", llvm::Intrinsic::experimental_constrained_fsub)
       .Case("llvm.experimental.constrained.llrint", llvm::Intrinsic::experimental_constrained_llrint)
       .Case("llvm.experimental.constrained.llround", llvm::Intrinsic::experimental_constrained_llround)
       .Case("llvm.experimental.constrained.log", llvm::Intrinsic::experimental_constrained_log)
       .Case("llvm.experimental.constrained.log10", llvm::Intrinsic::experimental_constrained_log10)
       .Case("llvm.experimental.constrained.log2", llvm::Intrinsic::experimental_constrained_log2)
       .Case("llvm.experimental.constrained.lrint", llvm::Intrinsic::experimental_constrained_lrint)
       .Case("llvm.experimental.constrained.lround", llvm::Intrinsic::experimental_constrained_lround)
       .Case("llvm.experimental.constrained.maximum", llvm::Intrinsic::experimental_constrained_maximum)
       .Case("llvm.experimental.constrained.maxnum", llvm::Intrinsic::experimental_constrained_maxnum)
       .Case("llvm.experimental.constrained.minimum", llvm::Intrinsic::experimental_constrained_minimum)
       .Case("llvm.experimental.constrained.minnum", llvm::Intrinsic::experimental_constrained_minnum)
       .Case("llvm.experimental.constrained.nearbyint", llvm::Intrinsic::experimental_constrained_nearbyint)
       .Case("llvm.experimental.constrained.pow", llvm::Intrinsic::experimental_constrained_pow)
       .Case("llvm.experimental.constrained.powi", llvm::Intrinsic::experimental_constrained_powi)
       .Case("llvm.experimental.constrained.rint", llvm::Intrinsic::experimental_constrained_rint)
       .Case("llvm.experimental.constrained.round", llvm::Intrinsic::experimental_constrained_round)
       .Case("llvm.experimental.constrained.sin", llvm::Intrinsic::experimental_constrained_sin)
       .Case("llvm.experimental.constrained.sitofp", llvm::Intrinsic::experimental_constrained_sitofp)
       .Case("llvm.experimental.constrained.sqrt", llvm::Intrinsic::experimental_constrained_sqrt)
       .Case("llvm.experimental.constrained.trunc", llvm::Intrinsic::experimental_constrained_trunc)
       .Case("llvm.experimental.constrained.uitofp", llvm::Intrinsic::experimental_constrained_uitofp)
       .Case("llvm.experimental.deoptimize", llvm::Intrinsic::experimental_deoptimize)
       .Case("llvm.experimental.gc.relocate", llvm::Intrinsic::experimental_gc_relocate)
       .Case("llvm.experimental.gc.result", llvm::Intrinsic::experimental_gc_result)
       .Case("llvm.experimental.gc.statepoint", llvm::Intrinsic::experimental_gc_statepoint)
       .Case("llvm.experimental.guard", llvm::Intrinsic::experimental_guard)
       .Case("llvm.experimental.patchpoint.i64", llvm::Intrinsic::experimental_patchpoint_i64)
       .Case("llvm.experimental.patchpoint.void", llvm::Intrinsic::experimental_patchpoint_void)
       .Case("llvm.experimental.stackmap", llvm::Intrinsic::experimental_stackmap)
       .Case("llvm.experimental.vector.reduce.add", llvm::Intrinsic::experimental_vector_reduce_add)
       .Case("llvm.experimental.vector.reduce.and", llvm::Intrinsic::experimental_vector_reduce_and)
       .Case("llvm.experimental.vector.reduce.fmax", llvm::Intrinsic::experimental_vector_reduce_fmax)
       .Case("llvm.experimental.vector.reduce.fmin", llvm::Intrinsic::experimental_vector_reduce_fmin)
       .Case("llvm.experimental.vector.reduce.mul", llvm::Intrinsic::experimental_vector_reduce_mul)
       .Case("llvm.experimental.vector.reduce.or", llvm::Intrinsic::experimental_vector_reduce_or)
       .Case("llvm.experimental.vector.reduce.smax", llvm::Intrinsic::experimental_vector_reduce_smax)
       .Case("llvm.experimental.vector.reduce.smin", llvm::Intrinsic::experimental_vector_reduce_smin)
       .Case("llvm.experimental.vector.reduce.umax", llvm::Intrinsic::experimental_vector_reduce_umax)
       .Case("llvm.experimental.vector.reduce.umin", llvm::Intrinsic::experimental_vector_reduce_umin)
       .Case("llvm.experimental.vector.reduce.v2.fadd", llvm::Intrinsic::experimental_vector_reduce_v2_fadd)
       .Case("llvm.experimental.vector.reduce.v2.fmul", llvm::Intrinsic::experimental_vector_reduce_v2_fmul)
       .Case("llvm.experimental.vector.reduce.xor", llvm::Intrinsic::experimental_vector_reduce_xor)
       .Case("llvm.experimental.widenable.condition", llvm::Intrinsic::experimental_widenable_condition)
       .Case("llvm.fabs", llvm::Intrinsic::fabs)
       .Case("llvm.floor", llvm::Intrinsic::floor)
       .Case("llvm.flt.rounds", llvm::Intrinsic::flt_rounds)
       .Case("llvm.fma", llvm::Intrinsic::fma)
       .Case("llvm.fmuladd", llvm::Intrinsic::fmuladd)
       .Case("llvm.frameaddress", llvm::Intrinsic::frameaddress)
       .Case("llvm.fshl", llvm::Intrinsic::fshl)
       .Case("llvm.fshr", llvm::Intrinsic::fshr)
       .Case("llvm.gcread", llvm::Intrinsic::gcread)
       .Case("llvm.gcroot", llvm::Intrinsic::gcroot)
       .Case("llvm.gcwrite", llvm::Intrinsic::gcwrite)
       .Case("llvm.get.dynamic.area.offset", llvm::Intrinsic::get_dynamic_area_offset)
       .Case("llvm.hwasan.check.memaccess", llvm::Intrinsic::hwasan_check_memaccess)
       .Case("llvm.hwasan.check.memaccess.shortgranules", llvm::Intrinsic::hwasan_check_memaccess_shortgranules)
       .Case("llvm.icall.branch.funnel", llvm::Intrinsic::icall_branch_funnel)
       .Case("llvm.init.trampoline", llvm::Intrinsic::init_trampoline)
       .Case("llvm.instrprof.increment", llvm::Intrinsic::instrprof_increment)
       .Case("llvm.instrprof.increment.step", llvm::Intrinsic::instrprof_increment_step)
       .Case("llvm.instrprof.value.profile", llvm::Intrinsic::instrprof_value_profile)
       .Case("llvm.invariant.end", llvm::Intrinsic::invariant_end)
       .Case("llvm.invariant.start", llvm::Intrinsic::invariant_start)
       .Case("llvm.is.constant", llvm::Intrinsic::is_constant)
       .Case("llvm.launder.invariant.group", llvm::Intrinsic::launder_invariant_group)
       .Case("llvm.lifetime.end", llvm::Intrinsic::lifetime_end)
       .Case("llvm.lifetime.start", llvm::Intrinsic::lifetime_start)
       .Case("llvm.llrint", llvm::Intrinsic::llrint)
       .Case("llvm.llround", llvm::Intrinsic::llround)
       .Case("llvm.load.relative", llvm::Intrinsic::load_relative)
       .Case("llvm.localaddress", llvm::Intrinsic::localaddress)
       .Case("llvm.localescape", llvm::Intrinsic::localescape)
       .Case("llvm.localrecover", llvm::Intrinsic::localrecover)
       .Case("llvm.log", llvm::Intrinsic::log)
       .Case("llvm.log10", llvm::Intrinsic::log10)
       .Case("llvm.log2", llvm::Intrinsic::log2)
       .Case("llvm.loop.decrement", llvm::Intrinsic::loop_decrement)
       .Case("llvm.loop.decrement.reg", llvm::Intrinsic::loop_decrement_reg)
       .Case("llvm.lrint", llvm::Intrinsic::lrint)
       .Case("llvm.lround", llvm::Intrinsic::lround)
       .Case("llvm.masked.compressstore", llvm::Intrinsic::masked_compressstore)
       .Case("llvm.masked.expandload", llvm::Intrinsic::masked_expandload)
       .Case("llvm.masked.gather", llvm::Intrinsic::masked_gather)
       .Case("llvm.masked.load", llvm::Intrinsic::masked_load)
       .Case("llvm.masked.scatter", llvm::Intrinsic::masked_scatter)
       .Case("llvm.masked.store", llvm::Intrinsic::masked_store)
       .Case("llvm.matrix.columnwise.load", llvm::Intrinsic::matrix_columnwise_load)
       .Case("llvm.matrix.columnwise.store", llvm::Intrinsic::matrix_columnwise_store)
       .Case("llvm.matrix.multiply", llvm::Intrinsic::matrix_multiply)
       .Case("llvm.matrix.transpose", llvm::Intrinsic::matrix_transpose)
       .Case("llvm.maximum", llvm::Intrinsic::maximum)
       .Case("llvm.maxnum", llvm::Intrinsic::maxnum)
       .Case("llvm.memcpy", llvm::Intrinsic::memcpy)
       .Case("llvm.memcpy.element.unordered.atomic", llvm::Intrinsic::memcpy_element_unordered_atomic)
       .Case("llvm.memmove", llvm::Intrinsic::memmove)
       .Case("llvm.memmove.element.unordered.atomic", llvm::Intrinsic::memmove_element_unordered_atomic)
       .Case("llvm.memset", llvm::Intrinsic::memset)
       .Case("llvm.memset.element.unordered.atomic", llvm::Intrinsic::memset_element_unordered_atomic)
       .Case("llvm.minimum", llvm::Intrinsic::minimum)
       .Case("llvm.minnum", llvm::Intrinsic::minnum)
       .Case("llvm.nearbyint", llvm::Intrinsic::nearbyint)
       .Case("llvm.objc.arc.annotation.bottomup.bbend", llvm::Intrinsic::objc_arc_annotation_bottomup_bbend)
       .Case("llvm.objc.arc.annotation.bottomup.bbstart", llvm::Intrinsic::objc_arc_annotation_bottomup_bbstart)
       .Case("llvm.objc.arc.annotation.topdown.bbend", llvm::Intrinsic::objc_arc_annotation_topdown_bbend)
       .Case("llvm.objc.arc.annotation.topdown.bbstart", llvm::Intrinsic::objc_arc_annotation_topdown_bbstart)
       .Case("llvm.objc.autorelease", llvm::Intrinsic::objc_autorelease)
       .Case("llvm.objc.autoreleasePoolPop", llvm::Intrinsic::objc_autoreleasePoolPop)
       .Case("llvm.objc.autoreleasePoolPush", llvm::Intrinsic::objc_autoreleasePoolPush)
       .Case("llvm.objc.autoreleaseReturnValue", llvm::Intrinsic::objc_autoreleaseReturnValue)
       .Case("llvm.objc.clang.arc.use", llvm::Intrinsic::objc_clang_arc_use)
       .Case("llvm.objc.copyWeak", llvm::Intrinsic::objc_copyWeak)
       .Case("llvm.objc.destroyWeak", llvm::Intrinsic::objc_destroyWeak)
       .Case("llvm.objc.initWeak", llvm::Intrinsic::objc_initWeak)
       .Case("llvm.objc.loadWeak", llvm::Intrinsic::objc_loadWeak)
       .Case("llvm.objc.loadWeakRetained", llvm::Intrinsic::objc_loadWeakRetained)
       .Case("llvm.objc.moveWeak", llvm::Intrinsic::objc_moveWeak)
       .Case("llvm.objc.release", llvm::Intrinsic::objc_release)
       .Case("llvm.objc.retain", llvm::Intrinsic::objc_retain)
       .Case("llvm.objc.retain.autorelease", llvm::Intrinsic::objc_retain_autorelease)
       .Case("llvm.objc.retainAutorelease", llvm::Intrinsic::objc_retainAutorelease)
       .Case("llvm.objc.retainAutoreleaseReturnValue", llvm::Intrinsic::objc_retainAutoreleaseReturnValue)
       .Case("llvm.objc.retainAutoreleasedReturnValue", llvm::Intrinsic::objc_retainAutoreleasedReturnValue)
       .Case("llvm.objc.retainBlock", llvm::Intrinsic::objc_retainBlock)
       .Case("llvm.objc.retainedObject", llvm::Intrinsic::objc_retainedObject)
       .Case("llvm.objc.storeStrong", llvm::Intrinsic::objc_storeStrong)
       .Case("llvm.objc.storeWeak", llvm::Intrinsic::objc_storeWeak)
       .Case("llvm.objc.sync.enter", llvm::Intrinsic::objc_sync_enter)
       .Case("llvm.objc.sync.exit", llvm::Intrinsic::objc_sync_exit)
       .Case("llvm.objc.unretainedObject", llvm::Intrinsic::objc_unretainedObject)
       .Case("llvm.objc.unretainedPointer", llvm::Intrinsic::objc_unretainedPointer)
       .Case("llvm.objc.unsafeClaimAutoreleasedReturnValue", llvm::Intrinsic::objc_unsafeClaimAutoreleasedReturnValue)
       .Case("llvm.objectsize", llvm::Intrinsic::objectsize)
       .Case("llvm.pcmarker", llvm::Intrinsic::pcmarker)
       .Case("llvm.pow", llvm::Intrinsic::pow)
       .Case("llvm.powi", llvm::Intrinsic::powi)
       .Case("llvm.prefetch", llvm::Intrinsic::prefetch)
       .Case("llvm.preserve.array.access.index", llvm::Intrinsic::preserve_array_access_index)
       .Case("llvm.preserve.struct.access.index", llvm::Intrinsic::preserve_struct_access_index)
       .Case("llvm.preserve.union.access.index", llvm::Intrinsic::preserve_union_access_index)
       .Case("llvm.ptr.annotation", llvm::Intrinsic::ptr_annotation)
       .Case("llvm.ptrmask", llvm::Intrinsic::ptrmask)
       .Case("llvm.read_register", llvm::Intrinsic::read_register)
       .Case("llvm.readcyclecounter", llvm::Intrinsic::readcyclecounter)
       .Case("llvm.returnaddress", llvm::Intrinsic::returnaddress)
       .Case("llvm.rint", llvm::Intrinsic::rint)
       .Case("llvm.round", llvm::Intrinsic::round)
       .Case("llvm.sadd.sat", llvm::Intrinsic::sadd_sat)
       .Case("llvm.sadd.with.overflow", llvm::Intrinsic::sadd_with_overflow)
       .Case("llvm.sdiv.fix", llvm::Intrinsic::sdiv_fix)
       .Case("llvm.set.loop.iterations", llvm::Intrinsic::set_loop_iterations)
       .Case("llvm.sideeffect", llvm::Intrinsic::sideeffect)
       .Case("llvm.sin", llvm::Intrinsic::sin)
       .Case("llvm.smul.fix", llvm::Intrinsic::smul_fix)
       .Case("llvm.smul.fix.sat", llvm::Intrinsic::smul_fix_sat)
       .Case("llvm.smul.with.overflow", llvm::Intrinsic::smul_with_overflow)
       .Case("llvm.sponentry", llvm::Intrinsic::sponentry)
       .Case("llvm.sqrt", llvm::Intrinsic::sqrt)
       .Case("llvm.ssa.copy", llvm::Intrinsic::ssa_copy)
       .Case("llvm.ssub.sat", llvm::Intrinsic::ssub_sat)
       .Case("llvm.ssub.with.overflow", llvm::Intrinsic::ssub_with_overflow)
       .Case("llvm.stackguard", llvm::Intrinsic::stackguard)
       .Case("llvm.stackprotector", llvm::Intrinsic::stackprotector)
       .Case("llvm.stackrestore", llvm::Intrinsic::stackrestore)
       .Case("llvm.stacksave", llvm::Intrinsic::stacksave)
       .Case("llvm.strip.invariant.group", llvm::Intrinsic::strip_invariant_group)
       .Case("llvm.test.set.loop.iterations", llvm::Intrinsic::test_set_loop_iterations)
       .Case("llvm.thread.pointer", llvm::Intrinsic::thread_pointer)
       .Case("llvm.trap", llvm::Intrinsic::trap)
       .Case("llvm.trunc", llvm::Intrinsic::trunc)
       .Case("llvm.type.checked.load", llvm::Intrinsic::type_checked_load)
       .Case("llvm.type.test", llvm::Intrinsic::type_test)
       .Case("llvm.uadd.sat", llvm::Intrinsic::uadd_sat)
       .Case("llvm.uadd.with.overflow", llvm::Intrinsic::uadd_with_overflow)
       .Case("llvm.udiv.fix", llvm::Intrinsic::udiv_fix)
       .Case("llvm.umul.fix", llvm::Intrinsic::umul_fix)
       .Case("llvm.umul.fix.sat", llvm::Intrinsic::umul_fix_sat)
       .Case("llvm.umul.with.overflow", llvm::Intrinsic::umul_with_overflow)
       .Case("llvm.usub.sat", llvm::Intrinsic::usub_sat)
       .Case("llvm.usub.with.overflow", llvm::Intrinsic::usub_with_overflow)
       .Case("llvm.va_copy", llvm::Intrinsic::vacopy)
       .Case("llvm.va_end", llvm::Intrinsic::vaend)
       .Case("llvm.va_start", llvm::Intrinsic::vastart)
       .Case("llvm.var.annotation", llvm::Intrinsic::var_annotation)
       .Case("llvm.write_register", llvm::Intrinsic::write_register)
       .Case("llvm.xray.customevent", llvm::Intrinsic::xray_customevent)
       .Case("llvm.xray.typedevent", llvm::Intrinsic::xray_typedevent)
       .Default(0);

   IntrinsicIDs[Name] = ID;
   return ID;
}

bool IRGen::intrinsicNeedsTypeAnnot(llvm::Intrinsic::ID ID)
{
   switch (ID) {
   default:
      return false;
   case llvm::Intrinsic::assume:
   case llvm::Intrinsic::bitreverse:
   case llvm::Intrinsic::bswap:
   case llvm::Intrinsic::canonicalize:
   case llvm::Intrinsic::ceil:
   case llvm::Intrinsic::copysign:
   case llvm::Intrinsic::cos:
   case llvm::Intrinsic::ctlz:
   case llvm::Intrinsic::ctpop:
   case llvm::Intrinsic::cttz:
   case llvm::Intrinsic::exp:
   case llvm::Intrinsic::exp2:
   case llvm::Intrinsic::expect:
   case llvm::Intrinsic::experimental_constrained_ceil:
   case llvm::Intrinsic::experimental_constrained_cos:
   case llvm::Intrinsic::experimental_constrained_exp:
   case llvm::Intrinsic::experimental_constrained_exp2:
   case llvm::Intrinsic::experimental_constrained_fadd:
   case llvm::Intrinsic::experimental_constrained_fcmp:
   case llvm::Intrinsic::experimental_constrained_fcmps:
   case llvm::Intrinsic::experimental_constrained_fdiv:
   case llvm::Intrinsic::experimental_constrained_floor:
   case llvm::Intrinsic::experimental_constrained_fma:
   case llvm::Intrinsic::experimental_constrained_fmul:
   case llvm::Intrinsic::experimental_constrained_fpext:
   case llvm::Intrinsic::experimental_constrained_fptosi:
   case llvm::Intrinsic::experimental_constrained_fptoui:
   case llvm::Intrinsic::experimental_constrained_fptrunc:
   case llvm::Intrinsic::experimental_constrained_frem:
   case llvm::Intrinsic::experimental_constrained_fsub:
   case llvm::Intrinsic::experimental_constrained_llrint:
   case llvm::Intrinsic::experimental_constrained_llround:
   case llvm::Intrinsic::experimental_constrained_log:
   case llvm::Intrinsic::experimental_constrained_log10:
   case llvm::Intrinsic::experimental_constrained_log2:
   case llvm::Intrinsic::experimental_constrained_lrint:
   case llvm::Intrinsic::experimental_constrained_lround:
   case llvm::Intrinsic::experimental_constrained_maximum:
   case llvm::Intrinsic::experimental_constrained_maxnum:
   case llvm::Intrinsic::experimental_constrained_minimum:
   case llvm::Intrinsic::experimental_constrained_minnum:
   case llvm::Intrinsic::experimental_constrained_nearbyint:
   case llvm::Intrinsic::experimental_constrained_pow:
   case llvm::Intrinsic::experimental_constrained_powi:
   case llvm::Intrinsic::experimental_constrained_rint:
   case llvm::Intrinsic::experimental_constrained_round:
   case llvm::Intrinsic::experimental_constrained_sin:
   case llvm::Intrinsic::experimental_constrained_sitofp:
   case llvm::Intrinsic::experimental_constrained_sqrt:
   case llvm::Intrinsic::experimental_constrained_trunc:
   case llvm::Intrinsic::experimental_constrained_uitofp:
   case llvm::Intrinsic::experimental_deoptimize:
   case llvm::Intrinsic::experimental_gc_relocate:
   case llvm::Intrinsic::experimental_gc_result:
   case llvm::Intrinsic::experimental_gc_statepoint:
   case llvm::Intrinsic::experimental_guard:
   case llvm::Intrinsic::experimental_patchpoint_i64:
   case llvm::Intrinsic::experimental_patchpoint_void:
   case llvm::Intrinsic::experimental_stackmap:
   case llvm::Intrinsic::experimental_vector_reduce_add:
   case llvm::Intrinsic::experimental_vector_reduce_and:
   case llvm::Intrinsic::experimental_vector_reduce_fmax:
   case llvm::Intrinsic::experimental_vector_reduce_fmin:
   case llvm::Intrinsic::experimental_vector_reduce_mul:
   case llvm::Intrinsic::experimental_vector_reduce_or:
   case llvm::Intrinsic::experimental_vector_reduce_smax:
   case llvm::Intrinsic::experimental_vector_reduce_smin:
   case llvm::Intrinsic::experimental_vector_reduce_umax:
   case llvm::Intrinsic::experimental_vector_reduce_umin:
   case llvm::Intrinsic::experimental_vector_reduce_v2_fadd:
   case llvm::Intrinsic::experimental_vector_reduce_v2_fmul:
   case llvm::Intrinsic::experimental_vector_reduce_xor:
   case llvm::Intrinsic::experimental_widenable_condition:
   case llvm::Intrinsic::fabs:
   case llvm::Intrinsic::floor:
   case llvm::Intrinsic::flt_rounds:
   case llvm::Intrinsic::fma:
   case llvm::Intrinsic::fmuladd:
   case llvm::Intrinsic::fshl:
   case llvm::Intrinsic::fshr:
   case llvm::Intrinsic::is_constant:
   case llvm::Intrinsic::lifetime_end:
   case llvm::Intrinsic::lifetime_start:
   case llvm::Intrinsic::llrint:
   case llvm::Intrinsic::llround:
   case llvm::Intrinsic::log:
   case llvm::Intrinsic::log10:
   case llvm::Intrinsic::log2:
   case llvm::Intrinsic::lrint:
   case llvm::Intrinsic::lround:
   case llvm::Intrinsic::matrix_columnwise_load:
   case llvm::Intrinsic::matrix_columnwise_store:
   case llvm::Intrinsic::matrix_multiply:
   case llvm::Intrinsic::matrix_transpose:
   case llvm::Intrinsic::maximum:
   case llvm::Intrinsic::maxnum:
   case llvm::Intrinsic::memcpy:
   case llvm::Intrinsic::memcpy_element_unordered_atomic:
   case llvm::Intrinsic::memmove:
   case llvm::Intrinsic::memmove_element_unordered_atomic:
   case llvm::Intrinsic::memset:
   case llvm::Intrinsic::memset_element_unordered_atomic:
   case llvm::Intrinsic::minimum:
   case llvm::Intrinsic::minnum:
   case llvm::Intrinsic::nearbyint:
   case llvm::Intrinsic::pow:
   case llvm::Intrinsic::powi:
   case llvm::Intrinsic::prefetch:
   case llvm::Intrinsic::rint:
   case llvm::Intrinsic::round:
   case llvm::Intrinsic::sadd_sat:
   case llvm::Intrinsic::sadd_with_overflow:
   case llvm::Intrinsic::sdiv_fix:
   case llvm::Intrinsic::set_loop_iterations:
   case llvm::Intrinsic::sin:
   case llvm::Intrinsic::smul_fix:
   case llvm::Intrinsic::smul_fix_sat:
   case llvm::Intrinsic::smul_with_overflow:
   case llvm::Intrinsic::sponentry:
   case llvm::Intrinsic::sqrt:
   case llvm::Intrinsic::ssub_sat:
   case llvm::Intrinsic::ssub_with_overflow:
   case llvm::Intrinsic::trunc:
   case llvm::Intrinsic::uadd_sat:
   case llvm::Intrinsic::uadd_with_overflow:
   case llvm::Intrinsic::udiv_fix:
   case llvm::Intrinsic::umul_fix:
   case llvm::Intrinsic::umul_fix_sat:
   case llvm::Intrinsic::umul_with_overflow:
   case llvm::Intrinsic::usub_sat:
   case llvm::Intrinsic::usub_with_overflow:
      return true;
   }
}