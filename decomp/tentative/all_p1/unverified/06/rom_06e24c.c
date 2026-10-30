/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06e24c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_14594(void)

{
  undefined4 uVar1;
  uint unaff_D7;
  int unaff_A5;
  int unaff_A6;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x30);
  *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)(unaff_A6 + 0x62);
  candidate_128e8();
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(undefined1 *)(unaff_A5 + 0x4009) = 0xff;
  candidate_4ed2();
  *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(undefined1 *)(unaff_A5 + 0x4009) = 0xff;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    if (((*(byte *)(unaff_A6 + 0x5f) & 0x20) == 0) && (*(char *)(unaff_A6 + 0x6d) == '@')) {
      *(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x7fe;
    }
    if (((*(byte *)(unaff_A6 + 0x5f) & 0x10) != 0) &&
       (*(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x28009800,
       *(char *)(unaff_A6 + 0x6d) != '@')) {
      *(uint *)(unaff_A5 + 0x4004) = *(uint *)(unaff_A5 + 0x4004) | 0x7fe;
    }
    candidate_2e98();
  }
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xf7;
  *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xef;
  func_0x0001448c();
  return unaff_D7 << 0x10 | unaff_D7 >> 0x10;
}
