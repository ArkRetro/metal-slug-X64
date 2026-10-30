/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049f9e.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x0004a01d) */

void candidate_49f9e(void)

{
  short in_D0w;
  int iVar1;
  undefined1 uVar2;
  int unaff_A6;
  undefined8 uVar3;

  if (0x13 < *(ushort *)(unaff_A6 + 0x6e)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  iVar1 = candidate_1b29c();
  if (iVar1 * 0x10000 + (uint)*(ushort *)(unaff_A6 + 0x90) * -0x100 == 0 ||
      iVar1 * 0x10000 < (int)((uint)*(ushort *)(unaff_A6 + 0x90) * 0x100)) {
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(uint *)(unaff_A6 + 0x3c) = (uint)*(ushort *)(unaff_A6 + 0x78) << 0x10;
    *(uint *)(unaff_A6 + 0x40) = (uint)*(ushort *)(unaff_A6 + 0x7a) << 0x10;
    *(undefined2 *)(unaff_A6 + 0x90) = 0;
  }
  else {
    *(short *)(unaff_A6 + 0x90) = in_D0w + *(short *)(unaff_A6 + 0x90);
                    /* WARNING: Bad instruction - Truncating control flow here */
    if (*(short *)(unaff_A6 + 0x76) < *(short *)(unaff_A6 + 0x90)) {
      *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x76);
    }
    uVar2 = candidate_2eb4();
    *(undefined1 *)(unaff_A6 + 0xaa) = uVar2;
    uVar3 = candidate_2e7c();
    *(undefined8 *)(unaff_A6 + 0x30) = uVar3;
  }
  return;
}
