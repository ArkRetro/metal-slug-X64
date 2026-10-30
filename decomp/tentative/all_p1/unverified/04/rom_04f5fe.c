/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f5fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f5fe(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
  *unaff_A6 = 0x4f636;
  candidate_4f9fa();
  func_0x000143e4();
  if ((*(char *)(unaff_A6 + 0x24) == '\0') && (0x15f < *(short *)(unaff_A6 + 0x10))) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8802;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8802;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
    sVar2 = candidate_49e8();
    bVar3 = false;
    if (sVar2 == 0) {
      candidate_4c24();
      if (bVar3) {
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
          *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
          candidate_120f8();
          *unaff_A6 = candidate_1c706;
          goto LAB_0004f71a;
        }
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
        *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
        candidate_120f8();
        *unaff_A6 = candidate_1c706;
      }
      candidate_4fa8e();
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
      candidate_120f8();
      *unaff_A6 = candidate_1c706;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    candidate_120f8();
    *unaff_A6 = candidate_1c706;
  }
LAB_0004f71a:
  candidate_4f8ac();
  return;
}
