/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01f002.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_1f002(void)

{
  byte bVar1;
  undefined2 uVar2;
  ushort uVar3;
  byte extraout_D1b;
  byte bVar4;
  int unaff_A6;

  candidate_c0644();
  uVar2 = candidate_c05b8();
  bVar4 = extraout_D1b;
  if ((char)uVar2 == '\x02') {
    bVar4 = extraout_D1b + 2;
  }
  bVar1 = *(byte *)(unaff_A6 + 0x88);
  *(byte *)(unaff_A6 + 0x88) = bVar4;
  bVar1 = bVar4 ^ bVar1;
  uVar3 = CONCAT11((char)((ushort)uVar2 >> 8),bVar1) & 0xff02;
  if ((bVar1 & 2) != 0) {
    if ((bVar4 & 2) == 0) {
      candidate_9f88();
      candidate_9ea8();
      candidate_9ea8();
      uVar3 = candidate_9ea8();
      *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
    }
    else {
      candidate_9f88();
      uVar3 = candidate_9ea8();
    }
  }
  return uVar3;
}
