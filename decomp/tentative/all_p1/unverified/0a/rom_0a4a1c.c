/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4a1c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4a1c(void)

{
  undefined4 uVar1;
  int unaff_A5;
  int unaff_A6;

  if (*(char *)(unaff_A5 + 0x4604) == '\0') {
    uVar1 = candidate_a4996();
    *(undefined4 *)(unaff_A6 + 0x84) = uVar1;
    uVar1 = candidate_a4996();
    *(undefined4 *)(unaff_A6 + 0x8c) = uVar1;
    *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)(unaff_A6 + 0x94);
    *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(unaff_A6 + 0x98);
  }
  else {
    uVar1 = candidate_a4996();
    *(undefined4 *)(unaff_A6 + 0x84) = uVar1;
    uVar1 = candidate_a4996();
    *(undefined4 *)(unaff_A6 + 0x8c) = uVar1;
    *(int *)(unaff_A6 + 0x30) = *(int *)(unaff_A6 + 0x94) << 2;
    *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x98) << 2;
  }
  return;
}
