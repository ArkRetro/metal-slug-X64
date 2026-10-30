/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042e22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42e22(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 200;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 200;
  *unaff_A6 = 0x42e50;
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
