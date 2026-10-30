/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04778e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4778e(void)

{
  undefined1 uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;

  uVar1 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x26) = uVar1;
  uVar1 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x97) = uVar1;
  *(undefined1 *)(unaff_A6 + 0x25) = *(undefined1 *)((int)unaff_A6 + 0x97);
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x477be;
  candidate_49e9e();
  if (*(char *)(unaff_A6 + 0x25) == '\0') {
    if (*(char *)(unaff_A6 + 0x26) == '\0') {
      *unaff_A6 = candidate_475ee;
    }
    else {
      *(char *)(unaff_A6 + 0x26) = *(char *)(unaff_A6 + 0x26) + -1;
      *(undefined1 *)(unaff_A6 + 0x25) = *(undefined1 *)((int)unaff_A6 + 0x97);
      candidate_40bc();
      *(undefined1 *)(extraout_A0 + 0x60) = 0;
    }
  }
  else {
    *(char *)(unaff_A6 + 0x25) = *(char *)(unaff_A6 + 0x25) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x96) < '\b') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + -3;
  func_0x000143e4();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x03';
  candidate_4900c();
  return;
}
