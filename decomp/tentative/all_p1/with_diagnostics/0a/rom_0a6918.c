/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6918.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

uint candidate_a6918(void)

{
  short sVar1;
  uint uVar2;
  undefined2 uVar3;
  ushort in_D0w;
  ushort uVar6;
  uint uVar4;
  uint uVar5;
  ushort in_D1w;
  ushort unaff_D2w;
  ushort unaff_D3w;

  if (unaff_D2w < unaff_D3w) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  uVar6 = in_D0w;
  if ((short)in_D0w < 0) {
    uVar6 = -in_D0w;
  }
  uVar4 = (uint)uVar6 * (uint)(ushort)(unaff_D2w - unaff_D3w);
  uVar2 = uVar4 / in_D1w;
  uVar4 = uVar4 % (uint)in_D1w;
  uVar5 = uVar4 << 0x10 | uVar2 & 0xffff;
  if (uVar2 != 0) {
    sVar1 = unaff_D3w + (short)uVar2;
    uVar3 = (undefined2)uVar4;
    uVar5 = CONCAT22(uVar3,sVar1);
    if ((short)in_D0w < 0) {
      uVar5 = CONCAT22(uVar3,-sVar1);
    }
  }
  return uVar5;
}
