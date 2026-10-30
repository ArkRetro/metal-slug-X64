/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0950dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_950dc(void)

{
  undefined4 *unaff_A6;
  undefined8 uVar1;

  candidate_120f8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe000;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe000;
  candidate_4256();
  candidate_9ea8();
  uVar1 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar1;
  unaff_A6[0x1c] = (int)((longlong)uVar1 >> 0x24);
  unaff_A6[0x1d] = (int)uVar1 >> 4;
  *(byte *)((int)unaff_A6 + 0x79) =
       (byte)((short)CONCAT31((int3)((longlong)uVar1 >> 0x2c),*(undefined1 *)((int)unaff_A6 + 0x6e))
             >> 3) & 0x1f;
  *unaff_A6 = candidate_95154;
  return;
}
