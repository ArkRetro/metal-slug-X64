/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05ef0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ef0a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  while( true ) {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    candidate_1436a();
    bVar2 = *(byte *)(unaff_A6 + 0x1b) < 3;
    if (*(byte *)(unaff_A6 + 0x1b) == 3) {
      candidate_427a();
      if ((bVar2) && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
        candidate_4256();
        *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
      }
      candidate_5fd48();
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      return;
    }
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    func_0x000143e4();
    uVar1 = *(byte *)(unaff_A6 + 0x1b) < 3;
    if (*(byte *)(unaff_A6 + 0x1b) != 3) break;
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    *unaff_A6 = candidate_5ef0a;
  }
  candidate_1abc8();
  candidate_5fa5c();
  candidate_5fb06();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5f21e;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
    *unaff_A6 = candidate_5ef5c;
  }
  return;
}
