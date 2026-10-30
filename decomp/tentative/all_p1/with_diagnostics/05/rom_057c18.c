/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x057c18.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

undefined8 candidate_57c18(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  short sVar4;
  uint *in_A0;
  uint *puVar5;

  if ('\x03' < *(char *)((int)in_A0 + 7)) {
    __m68k_trap(0xf);
    __m68k_trap(0xb);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  puVar5 = (uint *)0x10f010;
  uVar3 = 1;
  sVar4 = 1;
  uVar1 = *in_A0;
  while( true ) {
    uVar2 = *puVar5;
    if (uVar1 < uVar2) {
      uVar3 = (uint)(ushort)(sVar4 + 1);
    }
    if (uVar2 < uVar1) break;
    if ((uVar2 <= uVar1) && (sVar4 == 10)) {
      return 0xffffffffffffffff;
    }
    puVar5 = puVar5 + 2;
    sVar4 = sVar4 + 1;
    if (10 < sVar4) {
      return 0xffffffffffffffff;
    }
  }
  return CONCAT44(uVar3,(uint)(ushort)(sVar4 - 1));
}
