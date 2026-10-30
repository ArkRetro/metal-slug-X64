/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0711f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_711f8(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  *(undefined1 *)((int)unaff_A6 + 0x83) = 0xff;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  if (*(char *)((int)unaff_A6 + 0x87) == '\x12') {
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      candidate_353a4();
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x21) = 1;
      *unaff_A6 = 0x7124a;
      uVar2 = 0;
      candidate_726b8();
      candidate_7267c();
      candidate_1945c();
      if ((bool)uVar2) {
        *unaff_A6 = candidate_71d6e;
        uVar2 = 0;
      }
      uVar1 = candidate_aa610();
      if ((bool)uVar2) {
        *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
        bVar3 = false;
        uVar1 = candidate_aa760();
        if (bVar3) {
          *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
          candidate_aa83a();
          *unaff_A6 = candidate_71394;
        }
      }
      goto LAB_00071d66;
    }
    if (*(char *)((int)unaff_A6 + 0x87) == '\x12') {
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0x21) = 1;
      *unaff_A6 = 0x712d8;
      uVar2 = 0;
      candidate_726b8();
      candidate_7267c();
      if ((bool)uVar2) {
        *unaff_A6 = 0x71212;
        uVar2 = 0;
      }
      candidate_1945c();
      if ((bool)uVar2) {
        *unaff_A6 = candidate_71d6e;
      }
      goto LAB_00071d66;
    }
  }
  candidate_4256();
  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  *unaff_A6 = 0x71320;
  uVar2 = 0;
  candidate_726b8();
  candidate_7267c();
  if ((bool)uVar2) {
    *unaff_A6 = 0x71212;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_71d6e;
  }
LAB_00071d66:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
