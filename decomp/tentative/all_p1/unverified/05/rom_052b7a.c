/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052b7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_52b7a(void)

{
  int unaff_A6;
  bool bVar1;

  if (*(char *)(unaff_A6 + 0x84) == '\0') {
    candidate_1456a();
    if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
      *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0x5000;
      *(undefined1 *)(unaff_A6 + 0x84) = 0xff;
    }
  }
  else {
    *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x72) + *(int *)(unaff_A6 + 0x34);
    if (*(short *)(unaff_A6 + 0x2c) < 0xd9) {
      *(undefined1 *)(unaff_A6 + 0x85) = 0xff;
    }
    bVar1 = false;
    if (*(char *)(unaff_A6 + 0x85) < '\0') {
      candidate_52b5e();
      *(char *)(unaff_A6 + 0x84) = -!bVar1;
    }
    else {
      *(char *)(unaff_A6 + 0x85) = *(char *)(unaff_A6 + 0x85) + -1;
      candidate_1ac38();
    }
  }
  return;
}
