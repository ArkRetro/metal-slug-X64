/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b670.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b670(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x2b69e;
  candidate_1abc8();
  candidate_427a();
  bVar1 = *(byte *)(unaff_A6[3] + 0x60);
  if (bVar1 == 9) {
    *unaff_A6 = candidate_2b6e4;
  }
  uVar2 = bVar1 < 9;
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
    uVar2 = 0;
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
