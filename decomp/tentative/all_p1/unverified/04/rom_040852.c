/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x040852.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_40852(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  uint unaff_D6;
  undefined4 extraout_A1;
  int unaff_A6;
  undefined8 uVar3;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    *(int *)(unaff_A6 + 0x34) =
         (-0xc0000 - *(int *)(unaff_A6 + 0x34) >> 5) + *(int *)(unaff_A6 + 0x34);
    *(int *)(unaff_A6 + 0x30) =
         (*(int *)(unaff_A6 + 0x76) - *(int *)(unaff_A6 + 0x30) >> 5) + *(int *)(unaff_A6 + 0x30);
    *(undefined4 *)(unaff_A6 + 0x62) = *(undefined4 *)(unaff_A6 + 0x30);
    if (*(char *)(unaff_A6 + 0x74) == '\0') {
      if (((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0) || (*(char *)(unaff_A6 + 0x72) == '\0')) {
        func_0x000143e4();
      }
      else {
        candidate_143cc();
      }
      if ((unaff_D6 & 0x200) != 0) {
        *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x20;
      }
      if ((unaff_D6 & 1) != 0) {
        *(char *)(unaff_A6 + 0x72) = -((unaff_D6 & 8) != 0);
      }
    }
    else {
      uVar1 = *(undefined4 *)(unaff_A6 + 0x30);
      uVar2 = *(undefined4 *)(unaff_A6 + 0x34);
      uVar3 = candidate_1ac4c();
      *(undefined8 *)(unaff_A6 + 0x28) = uVar3;
      *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
      *(undefined4 *)(unaff_A6 + 0x34) = uVar2;
      *(undefined4 *)(unaff_A6 + 0x38) = extraout_A1;
      candidate_1ac38();
    }
  }
  else {
    *(int *)(unaff_A6 + 0x62) =
         (*(int *)(unaff_A6 + 0x76) - *(int *)(unaff_A6 + 0x62) >> 2) + *(int *)(unaff_A6 + 0x62);
    candidate_1456a();
    if ((unaff_D6 & 0x208) != 0) {
      *(undefined1 *)(unaff_A6 + 0x72) = 0xff;
    }
    if (((*(byte *)(unaff_A6 + 0x15) & 0x20) != 0) &&
       ((*(char *)(unaff_A6 + 0x6c) == '\a' || (*(char *)(unaff_A6 + 0x6c) == '\t')))) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xbf;
      *(undefined1 *)(unaff_A6 + 0x74) = 0xff;
    }
  }
  *(char *)(unaff_A6 + 0x70) = -((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0);
  return;
}
