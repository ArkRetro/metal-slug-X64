/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06914a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6914a(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_68d0c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_67d06;
  }
  else {
    if (*(char *)((int)unaff_A6 + 0x5a) == '\a') {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    }
    if (*(short *)((int)unaff_A6 + 0x7a) == 0) {
      *unaff_A6 = candidate_67c20;
    }
    else {
      *unaff_A6 = candidate_67d80;
    }
  }
  return;
}
