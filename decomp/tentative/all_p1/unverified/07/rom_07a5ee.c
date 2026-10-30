/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a5ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a5ee(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x800;
  candidate_1ad6e();
  candidate_1b1dc();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x97) = 3;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 200;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x24) = 0;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  candidate_40bc();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0xffff;
  candidate_4256();
  *unaff_A6 = 0x7a680;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  bVar1 = *(ushort *)((int)unaff_A6 + 0x82) < 4;
  if (*(ushort *)((int)unaff_A6 + 0x82) == 4) {
    candidate_7b7f2();
    if (bVar1) {
      *unaff_A6 = candidate_7a726;
    }
    if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7aa7e;
    }
    if (*(char *)(unaff_A6[3] + 0x61) == -1) {
      *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
      *unaff_A6 = 0x7aae8;
    }
  }
  else {
    if (*(short *)(unaff_A6[3] + 0x82) == 1) {
      *unaff_A6 = candidate_7a726;
    }
    if ((*(short *)(unaff_A6[3] + 0x82) == 2) &&
       (*(short *)(unaff_A6 + 0x24) = *(short *)(unaff_A6 + 0x24) + 1,
       4 < *(short *)(unaff_A6 + 0x24))) {
      *(undefined2 *)(unaff_A6 + 0x24) = 0;
      *(char *)((int)unaff_A6 + 0x97) = *(char *)((int)unaff_A6 + 0x97) + '\x01';
      if ('\x06' < *(char *)((int)unaff_A6 + 0x97)) {
        *(undefined1 *)((int)unaff_A6 + 0x97) = 6;
        *unaff_A6 = candidate_7a726;
      }
    }
  }
  return;
}
