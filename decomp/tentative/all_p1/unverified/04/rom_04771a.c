/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04771a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4771a(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  *unaff_A6 = 0x47720;
  candidate_49e9e();
  cVar1 = *(char *)((short)(*(ushort *)(unaff_A6 + 0x20) >> 4) + 0xe09f4);
  if (cVar1 == *(char *)((int)unaff_A6 + 0x96)) {
    *unaff_A6 = candidate_4778e;
  }
  else if (*(char *)((int)unaff_A6 + 0x96) < cVar1) {
    *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x01';
  }
  else {
    *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + -1;
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
