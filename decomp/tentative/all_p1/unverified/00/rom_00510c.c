/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00510c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_510c(void)

{
  short in_D0w;
  ushort uVar1;
  short in_D1w;
  int unaff_A5;

  if ((ushort)(in_D1w - 0x100U) < 0x100) {
    if ((ushort)(in_D0w - 0x20U) < 0x200) {
      uVar1 = *(ushort *)
               ((int)(short)((ushort)(in_D1w - 0x100U) >> 4 & 0xe) +
               (int)(short)((ushort)(in_D0w - 0x20U) >> 1 & 0xf0) + unaff_A5 + -0x181a);
      if (0xff < uVar1) {
        uVar1 = 0xff;
      }
      return uVar1;
    }
  }
  return 0;
}
