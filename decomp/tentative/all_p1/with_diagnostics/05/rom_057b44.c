/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057b44.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 candidate_57b44(void)

{
  uint uVar1;
  uint uVar2;
  ushort uVar4;
  uint uVar3;
  uint *in_A0;
  uint *puVar5;
  int unaff_A5;

  if ('\x03' < *(char *)((int)in_A0 + 7)) {
    __m68k_trap(0xf);
    __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  puVar5 = (uint *)0x10f010;
  uVar2 = 1;
  uVar4 = 1;
  uVar3 = *in_A0;
  while( true ) {
    uVar1 = *puVar5;
    if (uVar3 < uVar1) {
      uVar2 = (uint)(ushort)(uVar4 + 1);
    }
    if (uVar1 < uVar3) break;
    if ((uVar1 <= uVar3) && (uVar4 == 10)) {
      return 0xffffffffffffffff;
    }
    puVar5 = puVar5 + 2;
    uVar4 = uVar4 + 1;
    if (10 < (short)uVar4) {
      return 0xffffffffffffffff;
    }
  }
  uVar3 = (uint)(ushort)(uVar4 - 1);
  if (*(char *)((int)in_A0 + 7) < '\x04') {
    if (((in_A0 == (uint *)&DAT_0010c46a) && (*(char *)(unaff_A5 + 0x4469) != '\0')) &&
       (_DAT_0010c46a < *(uint *)(FUN_00004470 + unaff_A5 + 2))) {
      uVar2 = (uint)(ushort)((short)uVar2 + 1);
      uVar3 = (uint)uVar4;
    }
    if (((in_A0 == (uint *)&DAT_0010c472) && (FUN_00004464[unaff_A5 + 4] != (code)0x0)) &&
       (_DAT_0010c472 < *(uint *)(unaff_A5 + 0x446a))) {
      uVar2 = (uint)(ushort)((short)uVar2 + 1);
      uVar3 = (uint)(ushort)((short)uVar3 + 1);
    }
    return CONCAT44(uVar2,uVar3);
  }
  __m68k_trap(0xf);
  __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
