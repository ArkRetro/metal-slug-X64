/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05ab90.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5ab90(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  candidate_3fb10();
  sVar1 = 8;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar1 = -8;
  }
  *(short *)(unaff_A6 + 10) = sVar1 + *(short *)(unaff_A6 + 10);
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xfe;
  *unaff_A6 = 0x5abee;
  candidate_1456a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_5aca2;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_5aacc;
  }
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x1f) == '\0') {
    unaff_A6[0x14] = 0x5b75a;
  }
  else {
    unaff_A6[0x14] = 0;
  }
  bVar2 = false;
  candidate_4c24();
  if (!bVar2) goto LAB_0005ac9c;
  if (*(short *)((int)unaff_A6 + 0x72) != 0) {
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      if (((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) &&
         (sVar1 = *(short *)(unaff_A6 + 0x1d) + -1, *(short *)(unaff_A6 + 0x1d) = sVar1, sVar1 != 0)
         ) {
        *unaff_A6 = candidate_5ada8;
        goto LAB_0005ac9c;
      }
    }
    else if (((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) &&
            (sVar1 = *(short *)(unaff_A6 + 0x1d) + -1, *(short *)(unaff_A6 + 0x1d) = sVar1,
            sVar1 != 0)) {
      *unaff_A6 = candidate_5ada8;
      goto LAB_0005ac9c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *unaff_A6 = candidate_5aca2;
LAB_0005ac9c:
  candidate_5b714();
  return;
}
