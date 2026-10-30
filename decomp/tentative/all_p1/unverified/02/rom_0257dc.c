/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0257dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_257dc(void)

{
  undefined4 *unaff_A6;
  undefined8 uVar1;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  if (*(char *)(unaff_A6 + 0x27) < '\x05') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  uVar1 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar1;
  unaff_A6[0x20] = unaff_A6[0xd];
  *unaff_A6 = 0x2585c;
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    candidate_143dc();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      unaff_A6[0x20] = unaff_A6[0xd];
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      unaff_A6[0xd] = (int)-unaff_A6[0x20] >> 2;
    }
  }
  else {
    unaff_A6[0xd] = unaff_A6[0xd] + -0x53a0;
    candidate_1ac38();
  }
  candidate_427a();
  candidate_248d8();
  return;
}
