/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04aaae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4aaae(void)

{
  undefined4 *unaff_A6;

  *(undefined1 *)((int)unaff_A6 + 0x9e) = 0;
  if (*(char *)((int)unaff_A6 + 0xa1) == '\0') {
    *(undefined2 *)(unaff_A6 + 0x22) = 0x10;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x9d) = 3;
    *(undefined1 *)((int)unaff_A6 + 0xa1) = 0;
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
  }
  *unaff_A6 = 0x4aade;
  candidate_1abc8();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x9e) == '\0') {
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x9d) = 0x7f;
      *unaff_A6 = candidate_4ab10;
    }
    else {
      *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
    }
  }
  return;
}
