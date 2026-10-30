/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013204.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_13204(void)

{
  int unaff_D2;
  int unaff_D3;
  short unaff_D7w;
  int unaff_A5;

  if (unaff_D7w != 0x3c) {
    if (((unaff_D3 != 0 || unaff_D2 != 0) && ((short)*(undefined4 *)(unaff_A5 + 0x4024) != -1)) &&
       ((short)((uint)*(undefined4 *)(unaff_A5 + 0x4024) >> 0x10) != -1)) {
                    /* WARNING: Could not recover jumptable at 0x00012f1e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)(unaff_D7w + 0x12f22))();
      return;
    }
    return;
  }
  if (0 < unaff_D3) {
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xc000;
    *(undefined1 *)(unaff_A5 + 0x4026) = 0xff;
    return;
  }
  if (unaff_D3 < 0) {
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x4000;
    *(undefined1 *)(unaff_A5 + 0x4027) = 0xff;
    return;
  }
  return;
}
