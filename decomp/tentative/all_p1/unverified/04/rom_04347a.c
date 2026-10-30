/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04347a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4347a(void)

{
  byte bVar1;
  short sVar2;
  undefined4 *unaff_A6;

  sVar2 = *(short *)((int)unaff_A6 + 0x86) + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar2;
  if (sVar2 < 0) {
    *unaff_A6 = candidate_4349e;
  }
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
