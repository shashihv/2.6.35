#undef TRACE_SYSTEM
#define TRACE_SYSTEM writeback

#if !defined(_TRACE_WRITEBACK_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_WRITEBACK_H

#include <linux/backing-dev.h>
#include <linux/device.h>
#include <linux/writeback.h>

struct wb_writeback_work;

#define show_inode_state(state)					\
	__print_flags(state, "|",				\
		{I_DIRTY_SYNC,		"I_DIRTY_SYNC"},	\
		{I_DIRTY_DATASYNC,	"I_DIRTY_DATASYNC"},	\
		{I_DIRTY_PAGES,		"I_DIRTY_PAGES"},	\
		{I_NEW,			"I_NEW"},		\
		{I_WILL_FREE,		"I_WILL_FREE"},		\
		{I_FREEING,		"I_FREEING"},		\
		{I_CLEAR,		"I_CLEAR"},		\
		{I_SYNC,		"I_SYNC"},		\
		{I_REFERENCED,		"I_REFERENCED"}		\
		)

DECLARE_EVENT_CLASS(writeback_work_class,
	TP_PROTO(struct backing_dev_info *bdi, struct wb_writeback_work *work),
	TP_ARGS(bdi, work),
	TP_STRUCT__entry(
		__array(char, name, 32)
		__field(long, nr_pages)
		__field(dev_t, sb_dev)
		__field(int, sync_mode)
		__field(int, for_kupdate)
		__field(int, range_cyclic)
		__field(int, for_background)
	),
	TP_fast_assign(
		strncpy(__entry->name, dev_name(bdi->dev), 32);
		__entry->nr_pages = work->nr_pages;
		__entry->sb_dev = work->sb ? work->sb->s_dev : 0;
		__entry->sync_mode = work->sync_mode;
		__entry->for_kupdate = work->for_kupdate;
		__entry->range_cyclic = work->range_cyclic;
		__entry->for_background	= work->for_background;
	),
	TP_printk("bdi %s: sb_dev %d:%d nr_pages=%ld sync_mode=%d "
		  "kupdate=%d range_cyclic=%d background=%d",
		  __entry->name,
		  MAJOR(__entry->sb_dev), MINOR(__entry->sb_dev),
		  __entry->nr_pages,
		  __entry->sync_mode,
		  __entry->for_kupdate,
		  __entry->range_cyclic,
		  __entry->for_background
	)
);
#define DEFINE_WRITEBACK_WORK_EVENT(name) \
DEFINE_EVENT(writeback_work_class, name, \
	TP_PROTO(struct backing_dev_info *bdi, struct wb_writeback_work *work), \
	TP_ARGS(bdi, work))
DEFINE_WRITEBACK_WORK_EVENT(writeback_nothread);
DEFINE_WRITEBACK_WORK_EVENT(writeback_queue);
DEFINE_WRITEBACK_WORK_EVENT(writeback_exec);

TRACE_EVENT(writeback_pages_written,
	TP_PROTO(long pages_written),
	TP_ARGS(pages_written),
	TP_STRUCT__entry(
		__field(long,		pages)
	),
	TP_fast_assign(
		__entry->pages		= pages_written;
	),
	TP_printk("%ld", __entry->pages)
);

DECLARE_EVENT_CLASS(writeback_class,
	TP_PROTO(struct backing_dev_info *bdi),
	TP_ARGS(bdi),
	TP_STRUCT__entry(
		__array(char, name, 32)
	),
	TP_fast_assign(
		strncpy(__entry->name, dev_name(bdi->dev), 32);
	),
	TP_printk("bdi %s",
		  __entry->name
	)
);
#define DEFINE_WRITEBACK_EVENT(name) \
DEFINE_EVENT(writeback_class, name, \
	TP_PROTO(struct backing_dev_info *bdi), \
	TP_ARGS(bdi))

DEFINE_WRITEBACK_EVENT(writeback_nowork);
DEFINE_WRITEBACK_EVENT(writeback_wake_background);
DEFINE_WRITEBACK_EVENT(writeback_wake_thread);
DEFINE_WRITEBACK_EVENT(writeback_wake_forker_thread);
DEFINE_WRITEBACK_EVENT(writeback_bdi_register);
DEFINE_WRITEBACK_EVENT(writeback_bdi_unregister);
DEFINE_WRITEBACK_EVENT(writeback_thread_start);
DEFINE_WRITEBACK_EVENT(writeback_thread_stop);

DECLARE_EVENT_CLASS(wbc_class,
	TP_PROTO(struct writeback_control *wbc, struct backing_dev_info *bdi),
	TP_ARGS(wbc, bdi),
	TP_STRUCT__entry(
		__array(char, name, 32)
		__field(long, nr_to_write)
		__field(long, pages_skipped)
		__field(int, sync_mode)
		__field(int, for_kupdate)
		__field(int, for_background)
		__field(int, for_reclaim)
		__field(int, range_cyclic)
		__field(int, more_io)
		__field(unsigned long, older_than_this)
		__field(long, range_start)
		__field(long, range_end)
	),

	TP_fast_assign(
		strncpy(__entry->name, dev_name(bdi->dev), 32);
		__entry->nr_to_write	= wbc->nr_to_write;
		__entry->pages_skipped	= wbc->pages_skipped;
		__entry->sync_mode	= wbc->sync_mode;
		__entry->for_kupdate	= wbc->for_kupdate;
		__entry->for_background	= wbc->for_background;
		__entry->for_reclaim	= wbc->for_reclaim;
		__entry->range_cyclic	= wbc->range_cyclic;
		__entry->more_io	= wbc->more_io;
		__entry->older_than_this = wbc->older_than_this ?
						*wbc->older_than_this : 0;
		__entry->range_start	= (long)wbc->range_start;
		__entry->range_end	= (long)wbc->range_end;
	),

	TP_printk("bdi %s: towrt=%ld skip=%ld mode=%d kupd=%d "
		"bgrd=%d reclm=%d cyclic=%d more=%d older=0x%lx "
		"start=0x%lx end=0x%lx",
		__entry->name,
		__entry->nr_to_write,
		__entry->pages_skipped,
		__entry->sync_mode,
		__entry->for_kupdate,
		__entry->for_background,
		__entry->for_reclaim,
		__entry->range_cyclic,
		__entry->more_io,
		__entry->older_than_this,
		__entry->range_start,
		__entry->range_end)
)

#define DEFINE_WBC_EVENT(name) \
DEFINE_EVENT(wbc_class, name, \
	TP_PROTO(struct writeback_control *wbc, struct backing_dev_info *bdi), \
	TP_ARGS(wbc, bdi))
DEFINE_WBC_EVENT(wbc_writeback_start);
DEFINE_WBC_EVENT(wbc_writeback_written);
DEFINE_WBC_EVENT(wbc_writeback_wait);
DEFINE_WBC_EVENT(wbc_writepage);

TRACE_EVENT(writeback_single_inode,

	TP_PROTO(struct inode *inode,
		 struct writeback_control *wbc,
		 unsigned long wrote
	),

	TP_ARGS(inode, wbc, wrote),

	TP_STRUCT__entry(
		__array(char, name, 32)
		__field(unsigned long, ino)
		__field(unsigned long, state)
		__field(unsigned long, age)
		__field(unsigned long, wrote)
		__field(long, nr_to_write)
		__field(unsigned long, writeback_index)
	),

	TP_fast_assign(
		strncpy(__entry->name,
			dev_name(inode->i_mapping->backing_dev_info->dev), 32);
		__entry->ino		= inode->i_ino;
		__entry->state		= inode->i_state;
		__entry->age		= (jiffies - inode->dirtied_when) *
								1000 / HZ;
		__entry->wrote		= wrote;
		__entry->nr_to_write	= wbc->nr_to_write;
		__entry->writeback_index = inode->i_mapping->writeback_index;
	),

	TP_printk("bdi %s: ino=%lu state=%s age=%lu "
		  "wrote=%lu to_write=%ld index=%lu",
		  __entry->name,
		  __entry->ino,
		  show_inode_state(__entry->state),
		  __entry->age,
		  __entry->wrote,
		  __entry->nr_to_write,
		  __entry->writeback_index
	)
);

#define KBps(x)			((x) << (PAGE_SHIFT - 10))
#define Bps(x)			((x) >> (BASE_BW_SHIFT - PAGE_SHIFT))

TRACE_EVENT(dirty_throttle_bandwidth,

	TP_PROTO(struct backing_dev_info *bdi,
		 unsigned long dirty_bw,
		 unsigned long long pos_bw,
		 unsigned long long ref_bw),

	TP_ARGS(bdi, dirty_bw, pos_bw, ref_bw),

	TP_STRUCT__entry(
		__array(char,			bdi, 32)
		__field(unsigned long,		write_bw)
		__field(unsigned long,		avg_bw)
		__field(unsigned long,		dirty_bw)
		__field(unsigned long long,	base_bw)
		__field(unsigned long long,	pos_bw)
		__field(unsigned long long,	ref_bw)
		__field(unsigned long long,	avg_ref_bw)
	),

	TP_fast_assign(
		strlcpy(__entry->bdi, dev_name(bdi->dev), 32);
		__entry->write_bw	= KBps(bdi->write_bandwidth);
		__entry->avg_bw		= KBps(bdi->avg_bandwidth);
		__entry->dirty_bw	= KBps(dirty_bw);
		__entry->base_bw	= Bps(bdi->throttle_bandwidth);
		__entry->pos_bw		= Bps(pos_bw);
		__entry->ref_bw		= Bps(ref_bw);
		__entry->avg_ref_bw	= Bps(bdi->reference_bandwidth);
	),


	TP_printk("bdi %s: "
		  "write_bw=%lu avg_bw=%lu dirty_bw=%lu "
		  "base_bw=%llu pos_bw=%llu ref_bw=%llu aref_bw=%llu",
		  __entry->bdi,
		  __entry->write_bw,	/* bdi write bandwidth */
		  __entry->avg_bw,	/* bdi avg write bandwidth */
		  __entry->dirty_bw,	/* bdi dirty bandwidth */
		  __entry->base_bw,	/* base throttle bandwidth */
		  __entry->pos_bw,	/* position control bandwidth */
		  __entry->ref_bw,	/* reference throttle bandwidth */
		  __entry->avg_ref_bw	/* smoothed reference bandwidth */
	)
);

TRACE_EVENT(balance_dirty_pages,

	TP_PROTO(struct backing_dev_info *bdi,
		 unsigned long thresh,
		 unsigned long dirty,
		 unsigned long bdi_dirty,
		 unsigned long task_bw,
		 unsigned long dirtied,
		 unsigned long period,
		 long pause,
		 unsigned long start_time),

	TP_ARGS(bdi, thresh, dirty, bdi_dirty,
		task_bw, dirtied, period, pause, start_time),

	TP_STRUCT__entry(
		__array(	 char,	bdi, 32)
		__field(unsigned long,	bdi_weight)
		__field(unsigned long,	task_weight)
		__field(unsigned long,	limit)
		__field(unsigned long,	goal)
		__field(unsigned long,	dirty)
		__field(unsigned long,	bdi_goal)
		__field(unsigned long,	bdi_dirty)
		__field(unsigned long,	avg_dirty)
		__field(unsigned long,	base_bw)
		__field(unsigned long,	task_bw)
		__field(unsigned long,	dirtied)
		__field(unsigned long,	period)
		__field(	 long,	think)
		__field(	 long,	pause)
		__field(unsigned long,	paused)
	),

	TP_fast_assign(
		long numerator;
		long denominator;

		strlcpy(__entry->bdi, dev_name(bdi->dev), 32);

		bdi_writeout_fraction(bdi, &numerator, &denominator);
		__entry->bdi_weight	= 1000 * numerator / denominator;
		task_dirties_fraction(current, &numerator, &denominator);
		__entry->task_weight	= 1000 * numerator / denominator;

		__entry->limit = default_backing_dev_info.dirty_threshold;
		__entry->goal		= thresh - thresh / DIRTY_SCOPE;
		__entry->dirty		= dirty;
		__entry->bdi_goal	= bdi->dirty_threshold -
					  bdi->dirty_threshold / DIRTY_SCOPE;
		__entry->bdi_dirty	= bdi_dirty;
		__entry->avg_dirty	= bdi->avg_dirty;
		__entry->base_bw	= KBps(bdi->throttle_bandwidth) >>
								BASE_BW_SHIFT;
		__entry->task_bw	= KBps(task_bw);
		__entry->dirtied	= dirtied;
		__entry->think		= current->paused_when == 0 ? 0 :
			 (long)(jiffies - current->paused_when) * 1000 / HZ;
		__entry->period		= period * 1000 / HZ;
		__entry->pause		= pause * 1000 / HZ;
		__entry->paused		= (jiffies - start_time) * 1000 / HZ;
	),


	TP_printk("bdi %s: bdi_weight=%lu task_weight=%lu "
		  "limit=%lu goal=%lu dirty=%lu "
		  "bdi_goal=%lu bdi_dirty=%lu avg_dirty=%lu "
		  "base_bw=%lu task_bw=%lu "
		  "dirtied=%lu "
		  "period=%lu think=%ld pause=%ld paused=%lu",
		  __entry->bdi,
		  __entry->bdi_weight,
		  __entry->task_weight,
		  __entry->limit,
		  __entry->goal,
		  __entry->dirty,
		  __entry->bdi_goal,
		  __entry->bdi_dirty,
		  __entry->avg_dirty,
		  __entry->base_bw,	/* base throttle bandwidth */
		  __entry->task_bw,	/* task throttle bandwidth */
		  __entry->dirtied,
		  __entry->period,	/* ms */
		  __entry->think,	/* ms */
		  __entry->pause,	/* ms */
		  __entry->paused	/* ms */
		  )
);

TRACE_EVENT(global_dirty_state,

	TP_PROTO(unsigned long background_thresh,
		 unsigned long dirty_thresh
	),

	TP_ARGS(background_thresh,
		dirty_thresh
	),

	TP_STRUCT__entry(
		__field(unsigned long,	nr_dirty)
		__field(unsigned long,	nr_writeback)
		__field(unsigned long,	nr_unstable)
		__field(unsigned long,	background_thresh)
		__field(unsigned long,	dirty_thresh)
		__field(unsigned long,	poll_thresh)
		__field(unsigned long,	nr_dirtied)
		__field(unsigned long,	nr_written)
	),

	TP_fast_assign(
		__entry->nr_dirty	= global_page_state(NR_FILE_DIRTY);
		__entry->nr_writeback	= global_page_state(NR_WRITEBACK);
		__entry->nr_unstable	= global_page_state(NR_UNSTABLE_NFS);
		__entry->nr_dirtied	= global_page_state(NR_DIRTIED);
		__entry->nr_written	= global_page_state(NR_WRITTEN);
		__entry->background_thresh	= background_thresh;
		__entry->dirty_thresh		= dirty_thresh;
		__entry->poll_thresh		= current->nr_dirtied_pause;
	),

	TP_printk("dirty=%lu writeback=%lu unstable=%lu "
		  "bg_thresh=%lu thresh=%lu gap=%ld poll=%ld "
		  "dirtied=%lu written=%lu",
		  __entry->nr_dirty,
		  __entry->nr_writeback,
		  __entry->nr_unstable,
		  __entry->background_thresh,
		  __entry->dirty_thresh,
		  __entry->dirty_thresh - __entry->nr_dirty -
		  __entry->nr_writeback - __entry->nr_unstable,
		  __entry->poll_thresh,
		  __entry->nr_dirtied,
		  __entry->nr_written
	)
);

DECLARE_EVENT_CLASS(writeback_congest_waited_template,

	TP_PROTO(unsigned int usec_timeout, unsigned int usec_delayed),

	TP_ARGS(usec_timeout, usec_delayed),

	TP_STRUCT__entry(
		__field(	unsigned int,	usec_timeout	)
		__field(	unsigned int,	usec_delayed	)
	),

	TP_fast_assign(
		__entry->usec_timeout	= usec_timeout;
		__entry->usec_delayed	= usec_delayed;
	),

	TP_printk("usec_timeout=%u usec_delayed=%u",
			__entry->usec_timeout,
			__entry->usec_delayed)
);

DEFINE_EVENT(writeback_congest_waited_template, writeback_congestion_wait,

	TP_PROTO(unsigned int usec_timeout, unsigned int usec_delayed),

	TP_ARGS(usec_timeout, usec_delayed)
);

DEFINE_EVENT(writeback_congest_waited_template, writeback_wait_iff_congested,

	TP_PROTO(unsigned int usec_timeout, unsigned int usec_delayed),

	TP_ARGS(usec_timeout, usec_delayed)
);

#endif /* _TRACE_WRITEBACK_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
