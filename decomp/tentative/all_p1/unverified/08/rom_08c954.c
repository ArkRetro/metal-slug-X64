/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08c954.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8c954(void)

{
  undefined4 *unaff_A6;

  candidate_9ea8(*(undefined4 *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x86) << 2) + 0xe559e));
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
  candidate_8cf2e();
  candidate_4256();
  *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + *(short *)(unaff_A6 + 0x1e) * -8;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x10;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x50000;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 0;
  *unaff_A6 = 0x8c9fa;
  if (*(char *)(unaff_A6 + 0x21) == '\0') {
    *unaff_A6 = candidate_8ca12;
  }
  *(char *)(unaff_A6 + 0x21) = *(char *)(unaff_A6 + 0x21) + -1;
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x01';
  return;
}
