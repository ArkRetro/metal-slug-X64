/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x076802.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_76802(void)

{
  ushort uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;

  unaff_A6[0x14] = 0;
  if (*(char *)((int)unaff_A6 + 0x85) == '\x01') {
    *(undefined1 *)((int)unaff_A6 + 0x85) = 0xff;
  }
  candidate_767dc();
  uVar1 = candidate_29c6();
  *unaff_A6 = *(undefined4 *)(extraout_A0 + (short)((uVar1 & 7) << 2));
  return;
}
