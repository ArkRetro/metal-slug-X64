/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd3e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dd3e0(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  *unaff_A6 = 0xdd418;
  candidate_dd4b4();
  candidate_dd44c();
  candidate_1abc8();
  bVar1 = *(byte *)((int)unaff_A6 + 0xa5);
  bVar2 = 0xfe < bVar1;
  *(byte *)((int)unaff_A6 + 0xa5) = bVar1 + 1;
  candidate_44f6();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0xa5) = 1;
  }
  if ((*(byte *)((int)unaff_A6 + 0xa5) & 1) != 0) {
    candidate_427a();
  }
  return;
}
