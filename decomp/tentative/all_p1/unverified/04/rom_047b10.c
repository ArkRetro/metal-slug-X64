/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x047b10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_47b10(void)

{
  undefined2 uVar1;
  short sVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined8 uVar4;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  uVar4 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar4;
  candidate_4256();
  sVar3 = (ushort)*(byte *)(unaff_A6[3] + 0x96) << 2;
  sVar2 = *(short *)(sVar3 + 0xe0a1e);
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    sVar2 = -sVar2;
  }
  *(short *)(unaff_A6 + 10) = sVar2 + *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) = *(short *)(sVar3 + 0xe0a20) + *(short *)(unaff_A6 + 0xb);
  *unaff_A6 = 0x47bec;
  func_0x000143e4();
  if (*(short *)((int)unaff_A6 + 0x8a) == 0) {
    candidate_427a();
  }
  else {
    *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_48510;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_48510;
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_47c48;
  }
  candidate_49236();
  return;
}
