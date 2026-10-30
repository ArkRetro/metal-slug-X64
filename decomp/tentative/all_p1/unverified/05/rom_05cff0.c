/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05cff0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_5cff0(void)

{
  short sVar1;
  short sVar2;
  char cVar3;
  char cVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  if ((*(byte *)((int)unaff_A6 + 0x93) & 2) == 0) {
    candidate_4256();
    cVar4 = '\x10';
    bVar5 = SBORROW1(*(char *)(unaff_A6 + 0x22),'\b');
    cVar3 = *(char *)(unaff_A6 + 0x22) + -8;
  }
  else {
    candidate_4256();
    cVar4 = '\f';
    bVar5 = SBORROW1(*(char *)(unaff_A6 + 0x22),'\x06');
    cVar3 = *(char *)(unaff_A6 + 0x22) + -6;
  }
  if (bVar5 == cVar3 < '\0') {
    *(char *)(unaff_A6 + 0x22) = cVar4 - *(char *)(unaff_A6 + 0x22);
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  *unaff_A6 = 0x5d052;
  candidate_5e234();
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    *unaff_A6 = candidate_5d08e;
  }
  bVar5 = false;
  if ((_DAT_0010000e & 1) == 0) {
    cVar3 = *(char *)(unaff_A6 + 0x22);
    bVar5 = cVar3 == '\0';
    *(char *)(unaff_A6 + 0x22) = cVar3 + -1;
  }
  candidate_5e244();
  if (bVar5) {
    *unaff_A6 = candidate_5cd08;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar5 = bVar5 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar5) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
