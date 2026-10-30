/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2e7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2e7a(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9ea8();
  sVar1 = *(short *)((short)(*(short *)((int)unaff_A6 + 0x8e) * 2) + 0xe8a4c);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    sVar1 = -sVar1;
  }
  *(short *)(unaff_A6 + 10) = *(short *)((int)unaff_A6 + 0x92) + sVar1;
  *(undefined2 *)(unaff_A6 + 0xb) = 0x4d8;
  uVar2 = candidate_29c6();
  *(ushort *)(unaff_A6 + 10) = ((uVar2 & 7) - 4) + *(short *)(unaff_A6 + 10);
  uVar2 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) - (uVar2 & 3);
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  *unaff_A6 = 0xd2ef4;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  uVar3 = 0;
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    candidate_49e8();
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
