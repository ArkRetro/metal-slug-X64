/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081ea4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_81ea4(void)

{
  int iVar1;
  char extraout_D1b;
  char extraout_D1b_00;
  char cVar2;
  int extraout_D1;
  undefined4 extraout_A0;
  int unaff_A6;
  bool bVar3;

  bVar3 = false;
  if ((*(char *)(unaff_A6 + 0x9a) == '\0') && (candidate_1af9a(), !bVar3)) {
    bVar3 = false;
    if (*(int *)(unaff_A6 + 0x7a) == 0) {
      candidate_81e36();
      if (!bVar3) {
        *(undefined4 *)(unaff_A6 + 0x7a) = extraout_A0;
        cVar2 = extraout_D1b;
        if (extraout_D1b == '\0') {
          *(undefined1 *)(unaff_A6 + 0x9d) = 2;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x9d) = 1;
        }
LAB_00081f0c:
        iVar1 = *(int *)(unaff_A6 + 0x6e);
        if (cVar2 == '\0') {
          iVar1 = -iVar1;
          *(undefined1 *)(unaff_A6 + 0x9c) = 2;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x9c) = 1;
        }
        *(int *)(unaff_A6 + 0x72) =
             (iVar1 - *(int *)(unaff_A6 + 0x72) >> 4) + *(int *)(unaff_A6 + 0x72);
        *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(unaff_A6 + 0x72);
        return;
      }
    }
    else {
      thunk_FUN_00081e40();
      cVar2 = extraout_D1b_00;
      if (!bVar3) goto LAB_00081f0c;
      *(undefined4 *)(unaff_A6 + 0x7a) = 0;
      *(undefined1 *)(unaff_A6 + 0x99) = 0;
      *(undefined4 *)(unaff_A6 + 0x76) = 0;
      *(undefined1 *)(unaff_A6 + 0x9d) = 3;
      *(undefined1 *)(unaff_A6 + 0x9a) = 0x1e;
    }
  }
  *(undefined4 *)(unaff_A6 + 0x7a) = 0;
  *(int *)(unaff_A6 + 0x72) = *(int *)(unaff_A6 + 0x72) - (*(int *)(unaff_A6 + 0x72) >> 4);
  *(char *)(unaff_A6 + 0x99) = *(char *)(unaff_A6 + 0x99) + '\x03';
  candidate_2e7c();
  iVar1 = *(int *)(unaff_A6 + 0x76);
  *(int *)(unaff_A6 + 0x76) = extraout_D1;
  *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x72) + (extraout_D1 - iVar1);
  *(undefined1 *)(unaff_A6 + 0x9c) = 0;
  return;
}
