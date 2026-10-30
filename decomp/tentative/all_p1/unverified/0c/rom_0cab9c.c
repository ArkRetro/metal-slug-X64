/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cab9c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_cab9c(void)

{
  int extraout_A0;
  int unaff_A6;

  candidate_ca570();
  DAT_0010fdc4 = 1;
  _DAT_0010fdce = 0x250;
  func_0x00c00468();
  if ((DAT_0010fdc6 == '\0') && ((_DAT_0010fdd0 & 1) != 0)) {
    candidate_1aa28();
    candidate_c81a6();
    candidate_9c0a();
    candidate_40bc();
    *(undefined4 *)(extraout_A0 + 0x6e) = 0xca896;
    *(undefined2 *)(extraout_A0 + 0x72) = 4;
    *(undefined2 *)(extraout_A0 + 0x74) = 4;
    return;
  }
  *(undefined1 *)(unaff_A6 + 0x60) = 0xff;
  return;
}
