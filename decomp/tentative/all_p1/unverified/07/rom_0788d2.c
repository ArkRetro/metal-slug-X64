/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0788d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_788d2(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x78920;
  candidate_1abc8();
  uVar1 = 0;
  if (*(char *)(unaff_A6[3] + 0x9a) != '\0') {
    uVar1 = 0;
    candidate_4256();
  }
  candidate_427a();
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
