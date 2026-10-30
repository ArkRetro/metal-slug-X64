/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09635e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9635e(void)

{
  char cVar1;
  int unaff_A6;

  if ((DAT_0010f000 & 0x80) != 0) {
    func_0x00009cce();
    *(short *)(unaff_A6 + 0x3c) =
         *(char *)(*(short *)(unaff_A6 + 0x72) + 0x96356) + 4 + *(short *)(unaff_A6 + 0x3c);
    *(short *)(unaff_A6 + 0x40) = *(short *)(unaff_A6 + 0x40) + -8;
    for (cVar1 = *(char *)(*(short *)(unaff_A6 + 0x72) + 0x9635a); cVar1 != '\0'; cVar1 = cVar1 + -1
        ) {
      candidate_427a();
      *(short *)(unaff_A6 + 0x3c) = *(short *)(unaff_A6 + 0x3c) + 8;
    }
    candidate_1abc8();
  }
  return;
}
