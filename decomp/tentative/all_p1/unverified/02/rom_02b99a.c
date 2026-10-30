/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b99a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b99a(void)

{
  undefined1 uVar3;
  ushort uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar3 = 0x1e;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar3 = 0x12;
  }
  *(undefined1 *)(unaff_A6 + 0x23) = uVar3;
  *(undefined1 *)((int)unaff_A6 + 0x8d) =
       *(undefined1 *)((short)(*(byte *)(unaff_A6[3] + 0x8d) & 3) + 0x2c532);
  uVar1 = (ushort)*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x90);
  if (uVar1 == 0) {
    uVar1 = 0x7fff;
  }
  *(ushort *)(unaff_A6 + 0x22) = uVar1;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 5;
  *(undefined2 *)((int)unaff_A6 + 0x62) = 0x200;
  candidate_120f8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *unaff_A6 = 0x2ba42;
  *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  if (0 < *(short *)(unaff_A6 + 0x22)) {
    candidate_2bfec();
    candidate_2c0c2();
    candidate_2c08c();
    candidate_2c140();
  }
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    candidate_120f8();
    *unaff_A6 = candidate_2b822;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(unaff_A6 + 0x23);
  candidate_427a();
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_2b822;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_120f8();
    *unaff_A6 = candidate_2b822;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
