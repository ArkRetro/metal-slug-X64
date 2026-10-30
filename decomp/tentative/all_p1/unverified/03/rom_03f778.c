/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f778.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f778(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x3f78e;
  uVar1 = 0;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar1) {
    *(int *)((int)unaff_A6 + 0x7a) = -*(int *)((int)unaff_A6 + 0x7a);
    *(ushort *)((int)unaff_A6 + 0x6e) = *(ushort *)((int)unaff_A6 + 0x6e) ^ 1;
    *unaff_A6 = candidate_3f60c;
  }
  candidate_3fada();
  return;
}
