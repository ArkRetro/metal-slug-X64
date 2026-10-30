/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae720.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae720(void)

{
  undefined1 in_D1b;
  short *psVar1;
  int unaff_A5;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x96) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x94) = 0xff;
  candidate_aa760();
  if ((char)*(undefined2 *)(unaff_A6 + 0x6e) == '\0') {
    psVar1 = (short *)(unaff_A5 + 0x4608);
  }
  else {
    psVar1 = (short *)(unaff_A5 + 0x462c);
  }
  if ((short)unaff_A6 == *psVar1) {
    *psVar1 = 0;
    *(undefined1 *)(psVar1 + 0xe) = 2;
    *(undefined1 *)((int)psVar1 + 0x1d) = in_D1b;
    if (*(char *)(unaff_A6 + 0x5a) != '\0') {
      *(char *)(psVar1 + 0xf) = *(char *)(unaff_A6 + 0x5a);
    }
  }
  return;
}
