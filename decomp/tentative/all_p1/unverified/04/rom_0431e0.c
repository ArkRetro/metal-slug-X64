/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0431e0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_431e0(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 300;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 300;
  candidate_1ad6e();
  *(undefined1 *)(*(int *)((int)unaff_A6 + 0x72) + 0x88) = 0;
  *unaff_A6 = 0x4323e;
  candidate_1abc8();
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
