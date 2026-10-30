/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046f5a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46f5a(void)

{
  short *psVar1;
  char cVar2;
  undefined4 *unaff_A6;

  cVar2 = candidate_46fae();
  if (cVar2 != *(char *)(unaff_A6 + 0x24)) {
    *(char *)(unaff_A6 + 0x24) = cVar2;
    candidate_4256();
  }
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_46fec;
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
