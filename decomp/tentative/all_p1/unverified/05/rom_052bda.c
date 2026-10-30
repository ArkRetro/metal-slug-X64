/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052bda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_52bda(void)

{
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  undefined4 extraout_A0_01;
  int unaff_A6;
  bool bVar1;

  if (*(char *)(unaff_A6 + 0x8d) != '\0') {
    *(char *)(unaff_A6 + 0x8d) = *(char *)(unaff_A6 + 0x8d) + -1;
  }
  bVar1 = false;
  if (-1 < *(int *)(unaff_A6 + 0x6e)) {
    candidate_1aeda();
    if (bVar1) {
      candidate_1affc();
      *(char *)(unaff_A6 + 0x8b) = -!bVar1;
      if (!bVar1) {
        *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0;
      }
      goto LAB_00052c4a;
    }
    *(undefined4 *)(unaff_A6 + 0x6e) = 0xffffffff;
  }
  bVar1 = false;
  candidate_1affc();
  *(char *)(unaff_A6 + 0x8b) = -!bVar1;
  if (bVar1) {
    candidate_1af9a();
    if (!bVar1) {
      *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0_01;
    }
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0_00;
  }
LAB_00052c4a:
  if (*(int *)(unaff_A6 + 0x6e) < 0) {
    *(undefined1 *)(unaff_A6 + 0x89) = 0xff;
  }
  else {
    *(char *)(unaff_A6 + 0x89) =
         -((short)(*(short *)(*(int *)(unaff_A6 + 0x6e) + 0x3c) - *(short *)(unaff_A6 + 0x3c)) < 0)
         ^ -((*(byte *)(unaff_A6 + 0x15) & 1) != 0);
  }
  return;
}
