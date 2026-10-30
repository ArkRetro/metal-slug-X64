/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f4da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1f4da(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8843;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8843;
  candidate_4256();
  *unaff_A6 = 0x1f510;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x80;
  if (*(short *)(unaff_A6[3] + 0x6e) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  }
  bVar1 = *(char *)(unaff_A6[3] + 0x60) != -1;
  if (!bVar1) {
    *unaff_A6 = candidate_1f6e0;
  }
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_1f75a;
  }
  return;
}
