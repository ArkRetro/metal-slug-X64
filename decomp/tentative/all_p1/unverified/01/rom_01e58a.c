/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e58a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e58a(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  candidate_1ef2e();
  unaff_A6[0xd] = 0;
  cVar1 = candidate_29c6();
  if (cVar1 < '\0') {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_4256();
  *unaff_A6 = candidate_1e604;
  return;
}
