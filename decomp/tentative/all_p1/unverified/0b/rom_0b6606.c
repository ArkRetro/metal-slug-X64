/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b6606.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b6606(void)

{
  ushort uVar1;
  byte bVar2;
  int extraout_A0;
  int extraout_A0_00;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    if (*(char *)((int)unaff_A6 + 0x5a) == '\f') {
      candidate_1da40();
      *(undefined2 *)(extraout_A0 + 0x66) = 0x8800;
      *(undefined2 *)(extraout_A0 + 0x1e) = 0x8800;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = *(ushort *)((int)unaff_A6 + 0x76);
    in_CF = uVar1 < *(ushort *)((int)unaff_A6 + 0x4e);
    if ((short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)uVar1) {
      *(ushort *)((int)unaff_A6 + 0x76) = uVar1 >> 1;
      bVar2 = *(char *)((int)unaff_A6 + 0x79) + 1;
      if ('\x02' < (char)bVar2) {
        bVar2 = 2;
      }
      in_CF = bVar2 < *(byte *)((int)unaff_A6 + 0x79);
      if (bVar2 != *(byte *)((int)unaff_A6 + 0x79)) {
        *(byte *)((int)unaff_A6 + 0x79) = bVar2;
        if (bVar2 == 1) {
          candidate_4256();
        }
        else {
          candidate_4256();
        }
        candidate_1da40();
        *(undefined2 *)(extraout_A0_00 + 0x66) = 0x8800;
        *(undefined2 *)(extraout_A0_00 + 0x1e) = 0x8800;
        candidate_1dc1e();
        in_CF = 0;
        candidate_120f8();
      }
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_b66f2;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = 0xb6736;
  }
  return;
}
