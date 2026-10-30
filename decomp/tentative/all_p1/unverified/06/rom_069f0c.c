/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069f0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69f0c(void)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  *unaff_A6 = 0x69f32;
  if (*(short *)((int)unaff_A6 + 0x76) != 0) {
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0x7f);
  *(byte *)(unaff_A6[3] + 0x7f) = bVar1 & 0xfb;
  if ((bVar1 & 4) == 0) {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
    *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xf7;
    return;
  }
  candidate_1aca2();
  candidate_1ad6e();
  candidate_4256();
  uVar2 = 0x100000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = 0xfff00000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x6e) = uVar2;
  unaff_A6[0xc] = 0x200000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  unaff_A6[0xd] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  while( true ) {
    func_0x000143e4();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
      if (*(char *)((int)unaff_A6 + 0x81) != *(char *)(unaff_A6 + 0x20)) {
        *(char *)(unaff_A6 + 0x20) = *(char *)((int)unaff_A6 + 0x81);
        *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
      }
      if (*(short *)((int)unaff_A6 + 0x76) == 0) {
        candidate_411c();
        candidate_1b220();
        *(undefined2 *)((int)unaff_A6 + 0x76) = 0x14;
      }
      return;
    }
    if ((*(short *)(unaff_A6 + 0xf) < 0) || (0x140 < *(short *)(unaff_A6 + 0xf))) break;
    unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x6e);
    unaff_A6[0xd] = 0;
    if ((*(byte *)((int)unaff_A6 + 0x7f) & 8) == 0) {
      candidate_49e8();
    }
    candidate_427a();
    *(char *)((int)unaff_A6 + 0x81) = *(char *)((int)unaff_A6 + 0x81) + '\x01';
  }
  *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  return;
}
