/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0487d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_487d0(void)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined2 *)(unaff_A6 + 0x2a) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar3 = 0xe0af2;
  }
  else {
    iVar3 = 0xe0afe;
  }
  unaff_A6[0x28] = *(undefined4 *)(iVar3 + (short)(*(short *)(unaff_A6[3] + 0x70) * 4));
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar2;
  *unaff_A6 = 0x48832;
  *(short *)(unaff_A6 + 0x2a) = *(short *)(unaff_A6 + 0x2a) + 0x30;
  if (*(short *)((int)unaff_A6 + 0x76) <= *(short *)(unaff_A6 + 0x2a)) {
    *(undefined2 *)(unaff_A6 + 0x2a) = *(undefined2 *)((int)unaff_A6 + 0x76);
  }
  candidate_21288();
  iVar3 = unaff_A6[3];
  sVar1 = (ushort)*(byte *)((int)unaff_A6 + 0x61) * 4 + 0x76;
  *(undefined2 *)(iVar3 + sVar1) = *(undefined2 *)(unaff_A6 + 0xf);
  *(undefined2 *)(iVar3 + 2 + (int)sVar1) = *(undefined2 *)(unaff_A6 + 0x10);
  *(undefined1 *)(iVar3 + (short)(*(byte *)((int)unaff_A6 + 0x61) + 0x90)) =
       *(undefined1 *)((int)unaff_A6 + 0xaa);
  candidate_490ae();
  return;
}
