import { Component, OnInit, AfterViewInit, OnChanges, SimpleChanges, Input, ViewChild, ElementRef, HostListener } from '@angular/core';

@Component({
  selector: 'app-nonce-distribution',
  templateUrl: './nonce-distribution.component.html',
  styleUrls: ['./nonce-distribution.component.scss']
})
export class NonceDistributionComponent implements OnInit, AfterViewInit, OnChanges {

  @Input() nonceDistribution: number[] = [];
  @Input() asicCount: number = 4;
  @Input() scalingExponent: number = 0.5;

  private chipValues: number[] = [];

  @ViewChild('canvas', { static: true }) canvas!: ElementRef<HTMLCanvasElement>;
  private ctx!: CanvasRenderingContext2D;

  private chipMarkerRadius: number = 5;
  private scaledMarkerRadius: number = 8;

  tooltipText: string = '';
  tooltipVisible: boolean = false;
  tooltipX: number = 0;
  tooltipY: number = 0;

  constructor() {}

  ngOnInit(): void {
    this.updateChipValues();
  }

  ngAfterViewInit(): void {
    this.ctx = this.canvas.nativeElement.getContext('2d') as CanvasRenderingContext2D;
    this.drawBalanceVisualization();
  }

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['nonceDistribution'] || changes['asicCount']) {
      this.updateChipValues();
      this.drawBalanceVisualization();
    }
    if (changes['scalingExponent']) {
      this.drawBalanceVisualization();
    }
  }

  private updateChipValues(): void {
    this.chipValues = [...this.nonceDistribution];
    while (this.chipValues.length < this.asicCount) {
      this.chipValues.push(0);
    }
    this.chipValues = this.chipValues.slice(0, this.asicCount);
  }

      // Example method to compute a "quality percent" using normalized stdDev
    private computeQualityNormalized(chipValues: number[]): number {
    // 1) Handle edge case: if no values or all zero, just return 0% or 100% or your choice
    if (!chipValues.length) {
      return 0;
    }

    // 2) Find min and max in the data
    const minVal = Math.min(...chipValues);
    const maxVal = Math.max(...chipValues);

    // If all values are the same, min == max, so there's no spread
    if (minVal === maxVal) {
      // That means perfect uniform data => 100% quality
      return 100;
    }

    // 3) Normalize each value to [0..1] using min–max
    // normalizedValue = (value - minVal) / (maxVal - minVal)
    const range = maxVal - minVal;
    const normalizedValues = chipValues.map(v => (v - minVal) / range);

    // 4) Compute stdDev of these normalized values
    //    (Mean of normalized array => variance => sqrt => stdDev)
    const normMean = normalizedValues.reduce((acc, v) => acc + v, 0) / normalizedValues.length;

    let normVariance = 0;
    normalizedValues.forEach(val => {
      const diff = val - normMean;
      normVariance += diff * diff;
    });
    normVariance /= normalizedValues.length; // or (length - 1) for sample variance
    const normStdDev = Math.sqrt(normVariance);

    // 5) Decide on a "max" stdDev in normalized space for 0% quality.
    //    For min–max normalized data, the largest possible spread often occurs
    //    if half the points are at 0 and half are at 1, yielding stdDev near ~0.5 for big samples.
    //    We'll pick 0.5 as a typical "worst-case" for 4 chips.
    const maxStdDevNormalized = 0.5;

    // 6) Convert this normalized stdDev to a 0..100% "quality" scale
    //    If normStdDev >= 0.5 => 0% quality, if normStdDev = 0 => 100% quality
    const rawQuality = 1 - (normStdDev / maxStdDevNormalized);
    const clampedQuality = Math.max(0, rawQuality); // ensure it doesn't go negative
    const qualityPercent = clampedQuality * 100;

    return qualityPercent;
  }

  private drawBalanceVisualization(): void {
    if (!this.ctx) {
      return;
    }

    const canvasElem = this.canvas.nativeElement;
    const width = canvasElem.width;
    const height = canvasElem.height;
    const centerX = width / 2;
    const centerY = height / 2;
    const margin = 20;
    const outerRadius = (Math.min(width, height) / 2) - margin;

    // clear first
    this.ctx.clearRect(0, 0, width, height);

    this.ctx.beginPath();
    this.ctx.arc(centerX, centerY, outerRadius, 0, 2 * Math.PI);
    this.ctx.fillStyle = '#304562';
    this.ctx.fill();

    this.ctx.strokeStyle = '#1f2d40';
    this.ctx.lineWidth = 1;
    for (let i = 1; i <= 3; i++) {
      this.ctx.beginPath();
      this.ctx.arc(centerX, centerY, (outerRadius / 3) * i, 0, 2 * Math.PI);
      this.ctx.stroke();
    }

    for (let i = 0; i < this.asicCount; i++) {
      const angle = (2 * Math.PI / this.asicCount) * i;
      const xEnd = centerX + outerRadius * Math.cos(angle);
      const yEnd = centerY + outerRadius * Math.sin(angle);
      this.ctx.beginPath();
      this.ctx.moveTo(centerX, centerY);
      this.ctx.lineTo(xEnd, yEnd);
      this.ctx.stroke();
    }

    const chipPositions: { x: number; y: number; value: number }[] = [];
    let faulty = false;

    for (let i = 0; i < this.asicCount; i++) {
        const angle = (2 * Math.PI / this.asicCount) * i - Math.PI / 2;
        const x = centerX + outerRadius * Math.cos(angle);
        const y = centerY + outerRadius * Math.sin(angle);
        const value = this.chipValues[i] || 0;
        chipPositions.push({ x, y, value });

        if (value === 0) {
          faulty = true;
        }
    }

    this.ctx.beginPath();
    this.ctx.arc(centerX, centerY, outerRadius, 0, 2 * Math.PI);
    this.ctx.strokeStyle = '#64b5f6';
    this.ctx.lineWidth = 4;
    this.ctx.stroke();

    for (let i = 0; i < this.asicCount; i++) {
      const angle = (2 * Math.PI / this.asicCount) * i - Math.PI / 2;
      const x = centerX + outerRadius * Math.cos(angle);
      const y = centerY + outerRadius * Math.sin(angle);

      // Draw the chip marker
      this.ctx.beginPath();
      this.ctx.arc(x, y, this.chipMarkerRadius, 0, 2 * Math.PI);
      this.ctx.fillStyle = '#3498db';
      this.ctx.fill();
      this.ctx.strokeStyle = '#2980b9';
      this.ctx.lineWidth = 1;
      this.ctx.stroke();
    }

    let totalWeight = 0;
    let weightedX = 0;
    let weightedY = 0;
    chipPositions.forEach(pos => {
      totalWeight += pos.value;
      weightedX += pos.x * pos.value;
      weightedY += pos.y * pos.value;
    });
    const computedCenterX = weightedX / totalWeight;
    const computedCenterY = weightedY / totalWeight;

    const dx = computedCenterX - centerX;
    const dy = computedCenterY - centerY;
    const r = Math.sqrt(dx * dx + dy * dy);

    let scaledCenterX = centerX;
    let scaledCenterY = centerY;
    if (r !== 0) {
      const scaledR = outerRadius * Math.pow(r / outerRadius, this.scalingExponent);
      scaledCenterX = centerX + (dx / r) * scaledR;
      scaledCenterY = centerY + (dy / r) * scaledR;
    }

    // Draw a dashed white line from geometric center to scaled center
    this.ctx.setLineDash([5, 5]);
    this.ctx.beginPath();
    this.ctx.moveTo(centerX, centerY);
    this.ctx.lineTo(scaledCenterX, scaledCenterY);
    this.ctx.strokeStyle = '#FFFFFF';
    this.ctx.lineWidth = 1;
    this.ctx.stroke();
    this.ctx.setLineDash([]);

    const qualityPercent = this.computeQualityNormalized(this.chipValues);

    console.log(`Quality: ${qualityPercent.toFixed(2)}%`);

    const qualityFraction = qualityPercent / 100;  // e.g. 0.0 → 1.0
    const minRadius = 3;    // radius at 100% quality
    const maxRadius = 50;   // radius at 0% quality

    const radius = minRadius
    + (maxRadius - minRadius) * (1 - qualityFraction);

    // 3. Use `dynamicRadius` for your circle
    const distance = Math.sqrt((scaledCenterX - centerX) ** 2 + (scaledCenterY - centerY) ** 2);
    const gradientFactor = distance / outerRadius;
    const red = Math.floor(gradientFactor * 255);
    const green = Math.floor((1 - gradientFactor) * 255);

    const color = faulty ? '#ff0000' : `rgb(${red}, ${green}, 0)`;

    this.ctx.save();  // save current drawing state
    this.ctx.beginPath();
    this.ctx.arc(centerX, centerY, outerRadius - 2, 0, 2 * Math.PI);
    this.ctx.clip();  // anything drawn outside this circle won't appear

    this.ctx.beginPath();
    this.ctx.arc(scaledCenterX, scaledCenterY, radius, 0, 2 * Math.PI);
    this.ctx.fillStyle = color;
    this.ctx.fill();
    this.ctx.strokeStyle = '#71368a';
    this.ctx.stroke();

    this.ctx.restore();
  }


  @HostListener('mousemove', ['$event'])
  onMouseMove(event: MouseEvent): void {
    const rect = this.canvas.nativeElement.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    // Show tooltip on hover
    this.tooltipText = this.chipValues.join('/'); // Format as "123/126/200/100"
    this.tooltipX = x + 10;
    this.tooltipY = y - 10;
    this.tooltipVisible = true;
  }

  @HostListener('mouseleave')
  onMouseLeave(): void {
    this.tooltipVisible = false;
  }
}
