/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043740.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43740(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A5;
  int unaff_A6;
  undefined4 *puVar3;

  sVar2 = *(short *)((short)(*(short *)(unaff_A6 + 0x86) * 2) + 0x44fb8);
  uVar1 = (*(ushort *)(unaff_A5 + -0x7ff2) & 1) + 1;
  do {
    *(undefined4 *)((int)register0x0000003c + -4) = *(undefined4 *)(unaff_A6 + 0x3c);
    *(undefined4 *)((int)register0x0000003c + -8) = *(undefined4 *)(unaff_A6 + 0x40);
    *(short *)((int)register0x0000003c + -0xc) = sVar2;
    *(ushort *)((int)register0x0000003c + -0x10) = uVar1;
    *(short *)(unaff_A6 + 0x3c) = ((short)(sVar2 * uVar1) >> 4) + *(short *)(unaff_A6 + 0x3c);
    *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + uVar1 * -0x10;
    *(undefined4 *)((int)register0x0000003c + -0x14) = 0x4377c;
    candidate_49e8();
    uVar1 = *(ushort *)((int)register0x0000003c + -0x10);
    sVar2 = *(short *)((int)register0x0000003c + -0xe);
    puVar3 = (undefined4 *)((int)register0x0000003c + -8);
    *(undefined4 *)(unaff_A6 + 0x40) = *(undefined4 *)((int)register0x0000003c + -0xc);
    register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
    *(undefined4 *)(unaff_A6 + 0x3c) = *puVar3;
    uVar1 = uVar1 + 2;
  } while (uVar1 < 10);
  return;
}
