/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca35c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_ca35c(void)

{
  short in_D0w;
  undefined2 in_D1w;
  char unaff_D2b;
  int unaff_A5;

  if (in_D0w == 0) {
    _DAT_0010ea00 = in_D1w;
    *(char *)(unaff_A5 + 0x6a04) = -(unaff_D2b != '\0');
  }
  else {
    _DAT_0010ea02 = in_D1w;
    *(char *)(unaff_A5 + 0x6a05) = -(unaff_D2b != '\0');
  }
  if ((char)in_D0w == '\0') {
    *(undefined4 *)(unaff_A5 + 0x69f8) = 0xca438;
    return;
  }
  *(undefined4 *)(unaff_A5 + 0x69fc) = 0xca438;
  return;
}
