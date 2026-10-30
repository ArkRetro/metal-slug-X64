/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05cc94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5cc94(void)

{
  short sVar1;
  short sVar2;
  undefined4 *extraout_A0;
  undefined4 *puVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + '\x01';
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  bVar4 = false;
  candidate_1af9a();
  puVar3 = extraout_A0;
  if (bVar4) {
    puVar3 = unaff_A6;
  }
  if (*(ushort *)(puVar3 + 0x10) < *(ushort *)(unaff_A6 + 0x10)) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x5cce2;
  uVar5 = 0;
  candidate_5e234();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = 0x5c694;
    uVar5 = 0;
  }
  candidate_5e244();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar4 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar4 = bVar4 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar4) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
