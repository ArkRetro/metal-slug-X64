/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04357e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4357e(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  candidate_1abc8();
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0;
  candidate_427a();
  bVar1 = *(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x6e +
                   (int)(short)(ushort)*(byte *)((int)unaff_A6 + 0x83));
  if (bVar1 != *(byte *)(unaff_A6 + 0x21)) {
    *(byte *)(unaff_A6 + 0x21) = bVar1;
    *unaff_A6 = *(undefined4 *)
                 (*(int *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x83) << 2) + 0x453a6) +
                 (int)(short)((bVar1 & 0x7f) * 4));
  }
  *(undefined1 *)(unaff_A6 + 0x20) = *(undefined1 *)(*(int *)((int)unaff_A6 + 0x6e) + 0x80);
  return;
}
