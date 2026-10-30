/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dbc22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dbc22(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(undefined2 *)(unaff_A6 + 0x1c) = 0x50;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x3c;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0x14] = 0xdc0e8;
  *unaff_A6 = 0xdbc6a;
  candidate_1abc8();
  bVar1 = *(char *)(unaff_A6 + 0x18) == '\0';
  if ((*(char *)(unaff_A6 + 0x18) < '\x01') &&
     (bVar1 = *(ushort *)((int)unaff_A6 + 0x4e) < 0x1e,
     (short)*(ushort *)((int)unaff_A6 + 0x4e) < 0x1f)) {
    candidate_1da40();
    candidate_dbbf4();
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    bVar1 = false;
  }
  candidate_4c24();
  if (bVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_1da40();
    candidate_1da40();
    *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
    *unaff_A6 = candidate_dbd06;
  }
  bVar1 = false;
  candidate_da488();
  if (bVar1) {
    *(byte *)(unaff_A6[3] + 0x61) = *(byte *)(unaff_A6[3] + 0x61) | 0xf;
    *unaff_A6 = 0xdbec6;
  }
  return;
}
