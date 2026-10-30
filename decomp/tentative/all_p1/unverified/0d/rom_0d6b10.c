/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6b10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6b10(void)

{
  byte bVar1;
  undefined2 uVar2;
  short sVar3;
  int iVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x2a) = 0x400;
  *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + -0x38;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0xf;
  iVar4 = 0xe92ea;
  if ((*(char *)(unaff_A6 + 0x29) == '\0') &&
     (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0x79) == '\x05')) {
    candidate_4c1c();
    iVar4 = 0xe930a;
  }
  unaff_A6[0x28] =
       *(undefined4 *)
        ((short)((*(ushort *)(iVar4 + (short)((*(byte *)((int)unaff_A6 + 0x7d) & 0xf) * 2)) & 0xf)
                << 2) + 0xe92aa);
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  candidate_4256();
  *unaff_A6 = 0xd6bac;
  bVar5 = false;
  candidate_21288();
  if (bVar5) {
    *unaff_A6 = candidate_d6c66;
  }
  *(ushort *)((int)unaff_A6 + 0x82) = (ushort)(*(byte *)((int)unaff_A6 + 0xaa) >> 3);
  candidate_427a();
  sVar3 = (short)*(int *)((int)unaff_A6 + 0x96);
  if (*(int *)((int)unaff_A6 + 0x96) != 0) {
    sVar3 = candidate_49e8();
  }
  if (sVar3 != 0) {
    *unaff_A6 = candidate_d6a72;
  }
  bVar5 = false;
  candidate_4c24();
  if (bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x7a);
  if (bVar1 == 5) {
    *unaff_A6 = candidate_d6c66;
  }
  bVar5 = bVar1 < 5;
  candidate_19504();
  if (bVar5) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
